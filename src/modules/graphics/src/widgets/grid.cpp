
#include "battery/graphics/widgets/grid.hpp"

namespace b::widgets {

    void grid::operator()() {}

    void grid::operator()(const std::function<void(std::function<void(int, int, std::function<void()>)>)>& grid_callback) {

        calc_widths();
        calc_heights();

        grid_container.native_window_border = frame_border;
        grid_container.width = desired_size().x;
        grid_container.height = desired_size().y;
        grid_container.flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        // This callback is the actual container of the grid itself
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        set_cursor_position();
        grid_container([this, &grid_callback]() {
            ImGui::PopStyleVar();

            // This callback runs once for the grid and runs the user code, which then calls the inner callback
            grid_callback([this] (int column, int row, const std::function<void()>& cell_content) {
                // This lambda function is called by the user code for each cell

                if (column >= widths.size()) {
                    b::log::error("b::widgets::grid: Column index out of bounds: {} >= {}", column, widths.size());
                    return;
                }

                if (row >= heights.size()) {
                    b::log::error("b::widgets::grid: Row index out of bounds: {} >= {}", row, heights.size());
                    return;
                }

                ImGui::PushID(get_identifier().c_str());
                ImGui::PushID(column);
                ImGui::PushID(row);

                worker_cell.left = 0;
                for (size_t i = 0; i < column; i++) worker_cell.left = worker_cell.left.numeric() + widths[i];
                worker_cell.top = 0;
                for (size_t i = 0; i < row; i++) worker_cell.top = worker_cell.top.numeric() + heights[i];

                worker_cell.native_window_border = cell_border;
                worker_cell.width = widths[column];
                worker_cell.height = heights[row];
                worker_cell([&cell_content]() {
                    cell_content();
                });

                ImGui::PopID();
                ImGui::PopID();
                ImGui::PopID();
            });
        });
        actual_position = grid_container.actual_position;
        actual_size = grid_container.actual_size;
    }

    void grid::calc_widths() {
        auto number_of_columns = cell_widths.size();
        widths.resize(number_of_columns);

        // Clear values
        std::fill(widths.begin(), widths.end(), 0.f);

        // First apply all absolute values
        float absolute_sum = 0;
        for (size_t i = 0; i < number_of_columns; i++) {
            if (cell_widths[i].unit() == b::unit::PIXEL) {
                widths[i] = cell_widths[i].numeric();                       // - ImGui::GetStyle().ItemSpacing.x;
                absolute_sum += cell_widths[i].numeric();
            }
        }

        // If all absolute columns already fill the page, we're done
        if (absolute_sum >= actual_size.x) {
            return;
        }

        // Now fill in the relative PERCENT columns
        float percent_sum = 0.f;
        for (size_t i = 0; i < number_of_columns; i++) {
            if (cell_widths[i].unit() == b::unit::PERCENT) {
                float proportion = std::clamp(cell_widths[i].numeric() / 100.f, 0.f, 1.f);
                widths[i] = actual_size.x * proportion;                  // - ImGui::GetStyle().ItemSpacing.x;
                percent_sum += actual_size.x * proportion;
            }
        }

        // Stop if absolute and percent fill the page
        if (absolute_sum + percent_sum >= actual_size.x) {
            return;
        }

        // Now fill in the relative EM columns
        float em_sum = 0.f;
        for (size_t i = 0; i < number_of_columns; i++) {
            if (cell_widths[i].unit() == b::unit::PERCENT) {
                widths[i] = cell_widths[i].numeric() * ImGui::GetFontSize();                  // - ImGui::GetStyle().ItemSpacing.x;
                em_sum += cell_widths[i].numeric() * ImGui::GetFontSize();
            }
        }

        // Stop if absolute, percent and em fill the page
        if (absolute_sum + percent_sum + em_sum >= actual_size.x) {
            return;
        }

        // And finally, share the remaining space proportionally between the UNITLESS columns
        float unitless_sum = 0.f;
        for (size_t i = 0; i < number_of_columns; i++) {
            if (cell_widths[i].unit() == b::unit::UNITLESS) {
                unitless_sum += cell_widths[i].numeric();
            }
        }

        float remaining = actual_size.x - absolute_sum - percent_sum - em_sum;
        for (size_t i = 0; i < number_of_columns; i++) {
            if (cell_widths[i].unit() == b::unit::UNITLESS) {
                if (unitless_sum != 0) {
                    float proportion = cell_widths[i].numeric() / unitless_sum;
                    widths[i] = remaining * proportion;                     // - ImGui::GetStyle().ItemSpacing.x;
                }
            }
        }
    }

    void grid::calc_heights() {
        auto number_of_rows = cell_heights.size();
        heights.resize(number_of_rows);

        // Clear values
        std::fill(heights.begin(), heights.end(), 0.f);

        // First apply all absolute values
        float absolute_sum = 0;
        for (size_t i = 0; i < number_of_rows; i++) {
            if (cell_heights[i].unit() == b::unit::PIXEL) {
                heights[i] = cell_heights[i].numeric();                       // - ImGui::GetStyle().ItemSpacing.x;
                absolute_sum += cell_heights[i].numeric();
            }
        }

        // If all absolute columns already fill the page, we're done
        if (absolute_sum >= actual_size.y) {
            return;
        }

        // Now fill in the relative PERCENT columns
        float percent_sum = 0.f;
        for (size_t i = 0; i < number_of_rows; i++) {
            if (cell_heights[i].unit() == b::unit::PERCENT) {
                float proportion = std::clamp(cell_heights[i].numeric() / 100.f, 0.f, 1.f);
                heights[i] = actual_size.y * proportion;                  // - ImGui::GetStyle().ItemSpacing.x;
                percent_sum += actual_size.y * proportion;
            }
        }

        // Stop if absolute and percent fill the page
        if (absolute_sum + percent_sum >= actual_size.y) {
            return;
        }

        // Now fill in the relative EM columns
        float em_sum = 0.f;
        for (size_t i = 0; i < number_of_rows; i++) {
            if (cell_heights[i].unit() == b::unit::PERCENT) {
                heights[i] = cell_heights[i].numeric() * ImGui::GetFontSize();                  // - ImGui::GetStyle().ItemSpacing.x;
                em_sum += cell_heights[i].numeric() * ImGui::GetFontSize();
            }
        }

        // Stop if absolute, percent and em fill the page
        if (absolute_sum + percent_sum + em_sum >= actual_size.y) {
            return;
        }

        // And finally, share the remaining space proportionally between the UNITLESS columns
        float unitless_sum = 0.f;
        for (size_t i = 0; i < number_of_rows; i++) {
            if (cell_heights[i].unit() == b::unit::UNITLESS) {
                unitless_sum += cell_heights[i].numeric();
            }
        }

        float remaining = actual_size.y - absolute_sum - percent_sum - em_sum;
        for (size_t i = 0; i < number_of_rows; i++) {
            if (cell_heights[i].unit() == b::unit::UNITLESS) {
                if (unitless_sum != 0) {
                    float proportion = cell_heights[i].numeric() / unitless_sum;
                    heights[i] = remaining * proportion;                     // - ImGui::GetStyle().ItemSpacing.x;
                }
            }
        }
    }

}