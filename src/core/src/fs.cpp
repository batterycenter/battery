
#include "battery/core/fs.hpp"

namespace b::fs {

    // ========================================================
    // ================== Begin path class ====================
    // ========================================================

    path::path(const char* path) : m_path(std::bit_cast<char8_t*>(path)) {}
    path::path(const std::string& path) : m_path(std::u8string(path.begin(), path.end())) {}
    path::path(const std::u8string& path) : m_path(path) {}

    path& path::operator=(const std::string& path) {
        assign(path);
        return *this;
    }

    path& path::assign(const b::fs::path& path) {
        m_path = path.m_path;
        return *this;
    }

    path& path::append(const b::fs::path& path) {
        m_path.append(path.m_path.u8string());
        return *this;
    }

    path& path::operator/=(const b::fs::path& path) {
        return append(path);
    }

    path& path::concat(const b::fs::path& path) {
        m_path.concat(path.m_path.u8string());
        return *this;
    }

    path& path::operator+=(const b::fs::path& path) {
        return concat(path);
    }

    void path::clear() {
        m_path.clear();
    }

    path& path::make_preferred() {
        m_path.make_preferred();
        return *this;
    }

    path& path::make_absolute() {
        *this = b::fs::absolute(*this);
        return *this;
    }

    path& path::remove_filename() {
        m_path.remove_filename();
        return *this;
    }

    path& path::replace_filename(const b::fs::path& filename) {
        m_path.replace_filename(filename.m_path);
        return *this;
    }

    path& path::replace_extension(const b::fs::path& extension) {
        m_path.replace_extension(extension.m_path);
        return *this;
    }

    void path::swap(path& other) noexcept {
        m_path.swap(other.m_path);
    }

    std::string path::string() const {
        auto str = m_path.generic_u8string();
        return std::string(str.begin(), str.end());
    }

    std::string path::native_string() const {
        auto str = m_path.u8string();
        return std::string(str.begin(), str.end());
    }

    int path::compare(const path& other) const noexcept {
        return m_path.compare(other.m_path);
    }

    path path::lexically_normal() const {
        return { m_path.lexically_normal().u8string() };
    }

    path path::lexically_relative(const path& base) const {
        return { m_path.lexically_relative(base.std_path()).u8string() };
    }

    path path::lexically_proximate(const path& base) const {
        return { m_path.lexically_proximate(base.std_path()).u8string() };
    }

    path path::root_name() const {
        return { m_path.root_name().u8string() };
    }

    path path::root_directory() const {
        return { m_path.root_directory().u8string() };
    }

    path path::root_path() const {
        return { m_path.root_path().u8string() };
    }

    path path::relative_root_path() const {
        return { m_path.relative_path().u8string() };
    }

    path path::parent_path() const {
        return { m_path.parent_path().u8string() };
    }

    path path::filename() const {
        return { m_path.filename().u8string() };
    }

    path path::stem() const {
        return { m_path.stem().u8string() };
    }

    path path::extension() const {
        return { m_path.extension().u8string() };
    }

    path path::raw_extension() const {
        auto ext = extension().string();
        if (ext.empty()) {
            return {};
        }
        if (ext[0] == U'.') {
            return ext.substr(1);
        }
        return ext;
    }

    bool path::empty() const noexcept {
        return m_path.empty();
    }

    bool path::has_root_path() const {
        return m_path.has_root_path();
    }

    bool path::has_root_name() const {
        return m_path.has_root_name();
    }

    bool path::has_root_directory() const {
        return m_path.has_root_directory();
    }

    bool path::has_relative_root_path() const {
        return m_path.has_relative_path();
    }

    bool path::has_parent_path() const {
        return m_path.has_parent_path();
    }

    bool path::has_filename() const {
        return m_path.has_filename();
    }

    bool path::has_stem() const {
        return m_path.has_stem();
    }

    bool path::has_extension() const {
        return m_path.has_extension();
    }

    bool path::is_absolute() const {
        return m_path.is_absolute();
    }

    bool path::is_relative() const {
        return m_path.is_relative();
    }

    std::filesystem::path path::std_path() const {
        return m_path;
    }

    path::iterator path::begin() noexcept {
        return m_path.begin();
    }

    path::iterator path::end() noexcept {
        return m_path.end();
    }

    path::const_iterator path::begin() const noexcept {
        return m_path.begin();
    }

    path::const_iterator path::end() const noexcept {
        return m_path.end();
    }

    bool path::operator==(const path& rhs) const noexcept {
        return m_path == rhs.m_path;
    }

    bool path::operator!=(const path& rhs) const noexcept {
        return m_path != rhs.m_path;
    }

    bool path::operator<(const path& rhs) const noexcept {
        return m_path < rhs.m_path;
    }

    bool path::operator<=(const path& rhs) const noexcept {
        return m_path <= rhs.m_path;
    }

    bool path::operator>(const path& rhs) const noexcept {
        return m_path > rhs.m_path;
    }

    bool path::operator>=(const path& rhs) const noexcept {
        return m_path >= rhs.m_path;
    }

    std::strong_ordering path::operator<=>(const path& rhs) const noexcept {
        return m_path <=> rhs.m_path;
    }

    void swap(path& lhs, path& rhs) noexcept {
        lhs.swap(rhs);
    }

    path operator/(const fs::path& lhs, const fs::path& rhs) {
        auto path = lhs;
        path.append(rhs);
        return path;
    }

    path operator+(const fs::path& lhs, const fs::path& rhs) {
        auto path = lhs;
        path.concat(rhs);
        return path;
    }

    // ========================================================
    // =================== End path class =====================
    // ========================================================


    b::fs::path absolute(const b::fs::path& path) {
        auto result = try_absolute(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::path,std::error_code> try_absolute(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::absolute(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return b::fs::path(result.u8string());
    }

    b::fs::path canonical(const b::fs::path& path) {
        auto result = try_canonical(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::path,std::error_code> try_canonical(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::canonical(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return b::fs::path(result.u8string());
    }

    b::fs::path relative(const b::fs::path& path, const b::fs::path& base) {
        auto result = try_relative(path, base);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::path,std::error_code> try_relative(const b::fs::path& path, const b::fs::path& base) noexcept {
        std::error_code ec;
        auto result = std::filesystem::relative(path.std_path(), base.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return b::fs::path(result.u8string());
    }

    b::fs::path proximate(const b::fs::path& path, const b::fs::path& base) {
        auto result = try_proximate(path, base);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::path,std::error_code> try_proximate(const b::fs::path& path, const b::fs::path& base) noexcept {
        std::error_code ec;
        auto result = std::filesystem::proximate(path.std_path(), base.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return b::fs::path(result.u8string());
    }

    void copy(const b::fs::path& from, const b::fs::path& to,
              std::optional<b::fs::copy_options> options) {
        auto result = try_copy(from, to, options);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_copy(
            const b::fs::path& from,
            const b::fs::path& to,
            std::optional<b::fs::copy_options> options) noexcept {
        if (!b::fs::exists(to.parent_path())) {
            b::fs::create_directory(to.parent_path());
        }
        std::error_code ec;
        if (options.has_value()) {
            std::filesystem::copy(from.std_path(), to.std_path(), options.value(), ec);
        }
        else {
            std::filesystem::copy(from.std_path(), to.std_path(), ec);
        }
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    void copy_file(const b::fs::path& from, const b::fs::path& to,
            std::optional<b::fs::copy_options> options) {
        auto result = try_copy_file(from, to, options);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_copy_file(
            const b::fs::path& from,
            const b::fs::path& to,
            std::optional<b::fs::copy_options> options) noexcept {
        std::error_code ec;
        if (!b::fs::exists(to.parent_path())) {
            b::fs::create_directory(to.parent_path());
        }
        if (options.has_value()) {
            std::filesystem::copy_file(from.std_path(), to.std_path(), options.value(), ec);
        }
        else {
            std::filesystem::copy_file(from.std_path(), to.std_path(), ec);
        }
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    void copy_symlink(const b::fs::path& from, const b::fs::path& to) {
        auto result = try_copy_symlink(from, to);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_copy_symlink(
            const b::fs::path& from,
            const b::fs::path& to) noexcept {
        std::error_code ec;
        if (!b::fs::exists(to.parent_path())) {
            b::fs::create_directory(to.parent_path());
        }
        std::filesystem::copy_symlink(from.std_path(), to.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    bool create_directory(const b::fs::path& path) {
        auto result = try_create_directory(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_create_directory(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::create_directories(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    void create_hard_link(const b::fs::path& target, const b::fs::path& link) {
        auto result = try_create_hard_link(target, link);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_create_hard_link(const b::fs::path& target,
                                                                       const b::fs::path& link) noexcept {
        std::error_code ec;
        if (!b::fs::exists(link.parent_path())) {
            b::fs::create_directory(link.parent_path());
        }
        std::filesystem::create_hard_link(target.std_path(), link.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    void create_symlink(const b::fs::path& target, const b::fs::path& link) {
        auto result = try_create_symlink(target, link);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_create_symlink(const b::fs::path& target,
                                                                 const b::fs::path& link) noexcept {
        std::error_code ec;
        if (!b::fs::exists(link.parent_path())) {
            b::fs::create_directory(link.parent_path());
        }
        std::filesystem::create_symlink(target.std_path(), link.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    void create_directory_symlink(const b::fs::path& target, const b::fs::path& link) {
        auto result = try_create_directory_symlink(target, link);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_create_directory_symlink(const b::fs::path& target,
                                                                               const b::fs::path& link) noexcept {
        std::error_code ec;
        if (!b::fs::exists(link.parent_path())) {
            b::fs::create_directory(link.parent_path());
        }
        std::filesystem::create_directory_symlink(target.std_path(), link.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    b::fs::path current_path() {
        auto result = try_current_path();
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::path,std::error_code> try_current_path() noexcept {
        std::error_code ec;
        auto result = std::filesystem::current_path(ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return b::fs::path(result.u8string());
    }

    void current_path(const b::fs::path& path) {
        auto result = try_current_path(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_current_path(const b::fs::path& path) noexcept {
        std::error_code ec;
        std::filesystem::current_path(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    bool exists(const b::fs::path& path) {
        auto result = try_exists(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_exists(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::exists(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool exists(b::fs::file_status status) noexcept {
        return std::filesystem::exists(status);  // This function is already non-throwing
    }

    bool equivalent(const b::fs::path& path1, const b::fs::path& path2) {
        auto result = try_equivalent(path1, path2);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_equivalent(const b::fs::path& path1, const b::fs::path& path2) noexcept {
        std::error_code ec;
        auto result = std::filesystem::equivalent(path1.std_path(), path2.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    size_t file_size(const b::fs::path& path) {
        auto result = try_file_size(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<size_t,std::error_code> try_file_size(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::file_size(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    size_t hard_link_count(const b::fs::path& path) {
        auto result = try_hard_link_count(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<size_t,std::error_code> try_hard_link_count(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::hard_link_count(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    b::fs::file_time_type last_write_time(const b::fs::path& path) {
        auto result = try_last_write_time(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::file_time_type,std::error_code> try_last_write_time(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::last_write_time(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    void last_write_time(const b::fs::path& path, b::fs::file_time_type newTime) {
        auto result = try_last_write_time(path, newTime);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_last_write_time(const b::fs::path& path,
                                                                             b::fs::file_time_type newTime) noexcept {
        std::error_code ec;
        std::filesystem::last_write_time(path.std_path(), newTime, ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    void permissions(const b::fs::path& path, b::fs::perms perms, b::fs::perm_options opts) {
        auto result = try_permissions(path, perms, opts);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_permissions(const b::fs::path& path,
                                                                  b::fs::perms perms,
                                                                  b::fs::perm_options opts) noexcept {
        std::error_code ec;
        std::filesystem::permissions(path.std_path(), perms, opts, ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    b::fs::path read_symlink(const b::fs::path& path) {
        auto result = try_read_symlink(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::path,std::error_code> try_read_symlink(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::read_symlink(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return b::fs::path(result.u8string());
    }

    size_t remove(const b::fs::path& path) {
        auto result = try_remove(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<size_t,std::error_code> try_remove(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::remove_all(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    void rename(const b::fs::path& oldPath, const b::fs::path& newPath) {
        auto result = try_rename(oldPath, newPath);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_rename(const b::fs::path& oldPath,
                                                             const b::fs::path& newPath) noexcept {
        std::error_code ec;
        if (!b::fs::exists(newPath.parent_path())) {
            b::fs::create_directory(newPath.parent_path());
        }
        std::filesystem::rename(oldPath.std_path(), newPath.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    void resize_file(const b::fs::path& path, size_t newSize) {
        auto result = try_resize_file(path, newSize);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
    }

    std::expected<std::nullopt_t,std::error_code> try_resize_file(const b::fs::path& path, size_t newSize) noexcept {
        std::error_code ec;
        std::filesystem::resize_file(path.std_path(), newSize, ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return std::nullopt;
    }

    b::fs::space_info space(const b::fs::path& path) {
        auto result = try_space(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::space_info,std::error_code> try_space(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::space(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return b::fs::space_info{result.capacity, result.free, result.available};
    }

    b::fs::file_status status(const b::fs::path& path) {
        auto result = try_status(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::file_status,std::error_code> try_status(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::status(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    b::fs::file_status symlink_status(const b::fs::path& path) {
        auto result = try_symlink_status(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<b::fs::file_status,std::error_code> try_symlink_status(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::symlink_status(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_block_file(const b::fs::path& path) {
        auto result = try_is_block_file(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_block_file(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_block_file(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_block_file(b::fs::file_status status) noexcept {
        return std::filesystem::is_block_file(status);
    }

    bool is_character_file(const b::fs::path& path) {
        auto result = try_is_character_file(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_character_file(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_character_file(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_character_file(b::fs::file_status status) noexcept {
        return std::filesystem::is_character_file(status);
    }

    bool is_directory(const b::fs::path& path) {
        auto result = try_is_directory(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_directory(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_directory(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_directory(b::fs::file_status status) noexcept {
        return std::filesystem::is_directory(status);
    }

    bool is_empty(const b::fs::path& path) {
        auto result = try_is_empty(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_empty(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_empty(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_fifo(const b::fs::path& path) {
        auto result = try_is_fifo(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_fifo(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_fifo(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_fifo(b::fs::file_status status) noexcept {
        return std::filesystem::is_fifo(status);
    }

    bool is_other(const b::fs::path& path) {
        auto result = try_is_other(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_other(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_other(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_other(b::fs::file_status status) noexcept {
        return std::filesystem::is_other(status);
    }

    bool is_regular_file(const b::fs::path& path) {
        auto result = try_is_regular_file(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_regular_file(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_regular_file(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_regular_file(b::fs::file_status status) noexcept {
        return std::filesystem::is_regular_file(status);
    }

    bool is_socket(const b::fs::path& path) {
        auto result = try_is_socket(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_socket(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_socket(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_socket(b::fs::file_status status) noexcept {
        return std::filesystem::is_socket(status);
    }

    bool is_symlink(const b::fs::path& path) {
        auto result = try_is_symlink(path);
        if (!result) {
            throw b::filesystem_error(result.error());
        }
        return result.value();
    }

    std::expected<bool,std::error_code> try_is_symlink(const b::fs::path& path) noexcept {
        std::error_code ec;
        auto result = std::filesystem::is_symlink(path.std_path(), ec);
        if (ec) {
            return std::unexpected(ec);
        }
        return result;
    }

    bool is_symlink(b::fs::file_status status) noexcept {
        return std::filesystem::is_symlink(status);
    }

    bool status_known(b::fs::file_status status) noexcept {
        return std::filesystem::status_known(status);
    }

    std::expected<std::string, b::filesystem_error> try_read(const b::fs::path &path) {
        auto result = try_read_binary(path);
        if (!result) {
            return std::unexpected(result.error());
        }
        return std::string(result.value().begin(), result.value().end());
    }

    std::string read(const fs::path &path) {
        auto result = try_read(path);
        if (!result) {
            throw result.error();
        }
        return result.value();
    }

    std::expected<b::bytearray, b::filesystem_error> try_read_binary(const b::fs::path &path) {
        b::fs::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::unexpected(b::filesystem_error(
                    b::format("Failed loading file {}: {}", path, b::strerror(errno))
            ));
        }

        return b::bytearray(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    }

    b::bytearray read_binary(const fs::path &path) {
        auto result = try_read_binary(path);
        if (!result) {
            throw result.error();
        }
        return result.value();
    }

    std::expected<size_t, b::filesystem_error> try_write(const fs::path &path, const std::string& content) {
        b::fs::ofstream file(path, std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            return std::unexpected(b::filesystem_error(
                    b::format("Failed loading file {}: {}", path, b::strerror(errno))
            ));
        }

        file << content;
        return static_cast<size_t>(file.tellp());
    }

    std::expected<size_t, b::filesystem_error> try_write(const fs::path &path, const b::bytearray& content) {
        return try_write(path, std::string(content.begin(), content.end()));
    }

    size_t write(const fs::path &path, const std::string& content) {
        auto result = try_write(path, content);
        if (!result) {
            throw result.error();
        }
        return result.value();
    }

    size_t write(const fs::path &path, const b::bytearray& content) {
        return write(path, std::string(content.begin(), content.end()));
    }

} // namespace b::fs

// Specify stream operators to be able to use `b::fs::path` with CLI11
std::istringstream &operator>>(std::istringstream &in, b::fs::path &val) {
    std::string str;
    in >> str;
    val = b::fs::path(str);
    return in;
}

std::stringstream &operator<<(std::stringstream &in, b::fs::path &val) {
    in << val.string();
    return in;
}