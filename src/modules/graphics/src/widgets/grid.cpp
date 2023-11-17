
#include "battery/graphics/widgets/grid.hpp"
#include "battery/log.hpp"

namespace b::widgets {

    void grid::operator()() {
        b::log::warn("Warning: The operator() without arguments is not implemented for b::widgets::grid! Make sure to pass an argument when rendering a grid!");
    }

    void grid::operator()(const std::function<void(std::function<void(int, int, std::function<void()>)>)>& grid_callback) {
        basePushStyle();

        calcWidths();
        calcHeights();

        m_gridContainer.native_window_border = frameBorder;
        m_gridContainer.width = baseGetBBSize().x;
        m_gridContainer.height = baseGetBBSize().y;
        m_gridContainer.flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        // This callback is the actual container of the grid itself
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, b::Vec2(0, 0));
        baseSetCursorPositionToMinBB();
        m_gridContainer([this, &grid_callback]() {
            ImGui::PopStyleVar();

            basePopStyle();
            childrenStyle.push();

            // This callback runs once for the grid and runs the user code, which then calls the inner callback
            grid_callback([this] (int column, int row, const std::function<void()>& cell_content) {
                // This lambda function is called by the user code for each cell

                if (column >= m_widths.size()) {
                    b::log::error("b::widgets::grid: Column index out of bounds: {} >= {}", column, m_widths.size());
                    return;
                }

                if (row >= m_heights.size()) {
                    b::log::error("b::widgets::grid: Row index out of bounds: {} >= {}", row, m_heights.size());
                    return;
                }

                ImGui::PushID(baseGetIdentifier().c_str());
                ImGui::PushID(column);
                ImGui::PushID(row);

                m_workerCell.left = 0;
                for (size_t i = 0; i < column; i++) {
                    m_workerCell.left = m_workerCell.left.numeric() + m_widths[i];
                }
                m_workerCell.top = 0;
                for (size_t i = 0; i < row; i++) {
                    m_workerCell.top = m_workerCell.top.numeric() + m_heights[i];
                }

                m_workerCell.native_window_border = cellBorder;
                m_workerCell.width = m_widths[column];
                m_workerCell.height = m_heights[row];
                m_workerCell([&cell_content]() {
                    cell_content();
                });

                ImGui::PopID();
                ImGui::PopID();
                ImGui::PopID();
            });

            childrenStyle.pop();
        });
        actualPosition = m_gridContainer.actualPosition;
        actualSize = m_gridContainer.actualSize;
    }

    void grid::calcWidths() {
        auto numberOfColumns = cellWidths.size();
        m_widths.resize(numberOfColumns);

        // Clear values
        std::fill(m_widths.begin(), m_widths.end(), 0.0);

        // First apply all absolute values
        double absoluteSum = 0.0;
        for (size_t i = 0; i < numberOfColumns; i++) {
            if (cellWidths[i].unit() == b::Unit::PIXEL) {
                m_widths[i] = cellWidths[i].numeric();                       // - ImGui::GetStyle().ItemSpacing.x;
                absoluteSum += cellWidths[i].numeric();
            }
        }

        // If all absolute columns already fill the page, we're done
        if (absoluteSum >= actualSize.x) {
            return;
        }

        // Now fill in the relative PERCENT columns
        double percentSum = 0.0;
        for (size_t i = 0; i < numberOfColumns; i++) {
            if (cellWidths[i].unit() == b::Unit::PERCENT) {
                auto proportion = std::clamp(cellWidths[i].numeric() / 100.0, 0.0, 1.0);
                m_widths[i] = actualSize.x * proportion;                  // - ImGui::GetStyle().ItemSpacing.x;
                percentSum += actualSize.x * proportion;
            }
        }

        // Stop if absolute and percent fill the page
        if (absoluteSum + percentSum >= actualSize.x) {
            return;
        }

        // Now fill in the relative EM columns
        double emSum = 0.0;
        for (size_t i = 0; i < numberOfColumns; i++) {
            if (cellWidths[i].unit() == b::Unit::PERCENT) {
                m_widths[i] = cellWidths[i].numeric() * ImGui::GetFontSize();                  // - ImGui::GetStyle().ItemSpacing.x;
                emSum += cellWidths[i].numeric() * ImGui::GetFontSize();
            }
        }

        // Stop if absolute, percent and em fill the page
        if (absoluteSum + percentSum + emSum >= actualSize.x) {
            return;
        }

        // And finally, share the remaining space proportionally between the UNITLESS columns
        double unitlessSum = 0.0;
        for (size_t i = 0; i < numberOfColumns; i++) {
            if (cellWidths[i].unit() == b::Unit::UNITLESS) {
                unitlessSum += cellWidths[i].numeric();
            }
        }

        auto remaining = actualSize.x - absoluteSum - percentSum - emSum;
        for (size_t i = 0; i < numberOfColumns; i++) {
            if (cellWidths[i].unit() == b::Unit::UNITLESS) {
                if (unitlessSum != 0) {
                    auto proportion = cellWidths[i].numeric() / unitlessSum;
                    m_widths[i] = remaining * proportion;                     // - ImGui::GetStyle().ItemSpacing.x;
                }
            }
        }
    }

    void grid::calcHeights() {
        auto numberOfRows = cellHeights.size();
        m_heights.resize(numberOfRows);

        // Clear values
        std::fill(m_heights.begin(), m_heights.end(), 0.0);

        // First apply all absolute values
        double absoluteSum = 0;
        for (size_t i = 0; i < numberOfRows; i++) {
            if (cellHeights[i].unit() == b::Unit::PIXEL) {
                m_heights[i] = cellHeights[i].numeric();                       // - ImGui::GetStyle().ItemSpacing.x;
                absoluteSum += cellHeights[i].numeric();
            }
        }

        // If all absolute columns already fill the page, we're done
        if (absoluteSum >= actualSize.y) {
            return;
        }

        // Now fill in the relative PERCENT columns
        double percentSum = 0.0;
        for (size_t i = 0; i < numberOfRows; i++) {
            if (cellHeights[i].unit() == b::Unit::PERCENT) {
                auto proportion = std::clamp(cellHeights[i].numeric() / 100.0, 0.0, 1.0);
                m_heights[i] = actualSize.y * proportion;                  // - ImGui::GetStyle().ItemSpacing.x;
                percentSum += actualSize.y * proportion;
            }
        }

        // Stop if absolute and percent fill the page
        if (absoluteSum + percentSum >= actualSize.y) {
            return;
        }

        // Now fill in the relative EM columns
        double emSum = 0.0;
        for (size_t i = 0; i < numberOfRows; i++) {
            if (cellHeights[i].unit() == b::Unit::PERCENT) {
                m_heights[i] = cellHeights[i].numeric() * ImGui::GetFontSize();                  // - ImGui::GetStyle().ItemSpacing.x;
                emSum += cellHeights[i].numeric() * ImGui::GetFontSize();
            }
        }

        // Stop if absolute, percent and em fill the page
        if (absoluteSum + percentSum + emSum >= actualSize.y) {
            return;
        }

        // And finally, share the remaining space proportionally between the UNITLESS columns
        double unitlessSum = 0.0;
        for (size_t i = 0; i < numberOfRows; i++) {
            if (cellHeights[i].unit() == b::Unit::UNITLESS) {
                unitlessSum += cellHeights[i].numeric();
            }
        }

        auto remaining = actualSize.y - absoluteSum - percentSum - emSum;
        for (size_t i = 0; i < numberOfRows; i++) {
            if (cellHeights[i].unit() == b::Unit::UNITLESS) {
                if (unitlessSum != 0) {
                    auto proportion = cellHeights[i].numeric() / unitlessSum;
                    m_heights[i] = remaining * proportion;                     // - ImGui::GetStyle().ItemSpacing.x;
                }
            }
        }
    }

} // namespace b