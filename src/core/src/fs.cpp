
#include "battery/core/fs.hpp"

//#include <expected>
//std::expected<int, int> s;

namespace b::fs {

    /// ========================================================
    /// ================== Begin path class ====================
    /// ========================================================

    path::path(const b::string& path) : m_path(path.encode_u8()) {

    }

    path& path::operator=(const b::string& path) {
        return assign(path);
    }

    path& path::assign(const b::string& path) {
        m_path = path.encode_u8();
        return *this;
    }

    path& path::assign(const b::fs::path& path) {
        m_path = path.m_path;
        return *this;
    }

    path& path::append(const b::string& path) {
        m_path.append(path.encode_u8());
        return *this;
    }

    path& path::append(const b::fs::path& path) {
        m_path.append(path.m_path.u8string());
        return *this;
    }

    path& path::operator/=(const b::string& path) {
        return append(path);
    }

    path& path::operator/=(const b::fs::path& path) {
        return append(path);
    }

    path& path::concat(const b::string& path) {
        m_path.concat(path.encode_u8());
        return *this;
    }

    path& path::concat(const b::fs::path& path) {
        m_path.concat(path.m_path.u8string());
        return *this;
    }

    path& path::operator+=(const b::string& path) {
        return concat(path);
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

    path& path::replace_filename(const b::string& filename) {
        m_path.replace_filename(filename.encode_u8());
        return *this;
    }

    path& path::replace_filename(const b::fs::path& filename) {
        m_path.replace_filename(filename.m_path);
        return *this;
    }

    path& path::replace_extension(const b::string& extension) {
        m_path.replace_extension(extension.encode_u8());
        return *this;
    }

    path& path::replace_extension(const b::fs::path& extension) {
        m_path.replace_extension(extension.m_path);
        return *this;
    }

    void path::swap(path& other) noexcept {
        m_path.swap(other.m_path);
    }

    b::string path::string() const {
        return { m_path.u8string() };
    }

    b::string path::generic_string() const {
        return { m_path.generic_u8string() };
    }

    int path::compare(const path& other) const noexcept {
        return m_path.compare(other.m_path);
    }

    path path::lexically_normal() const {
        return { m_path.lexically_normal().u8string() };
    }

    path path::lexically_relative(const path& base) const {
        return { m_path.lexically_relative(base.string().encode_u8()).u8string() };
    }

    path path::lexically_proximate(const path& base) const {
        return { m_path.lexically_proximate(base.string().encode_u8()).u8string() };
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
        if (!ext.empty()) {
            if (ext[0] == U'.') {
                ext = ext.substr(1);
//                ext.erase(ext.begin());     // TODO: Correct this using more high-level string functions
            }
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

    /// ========================================================
    /// =================== End path class =====================
    /// ========================================================



    path absolute(const b::fs::path& path) {
        return { std::filesystem::absolute(path.std_path()).u8string() };
    }

    path absolute(const b::fs::path& path, std::error_code& ec) noexcept {
        return { std::filesystem::absolute(path.std_path(), ec).u8string() };
    }

    bool exists(const b::fs::path& path) {
        return std::filesystem::exists(path.std_path().u8string());
    }

    bool is_directory(const b::fs::path& path) {
        return std::filesystem::is_directory(path.std_path().u8string());
    }

    bool is_regular_file(const b::fs::path& path) {
        return std::filesystem::is_regular_file(path.std_path().u8string());
    }

    bool create_directory(const b::fs::path& path) {
        return std::filesystem::create_directory(path.std_path().u8string());
    }

    std::filesystem::file_time_type last_write_time(const b::fs::path& path) {
        return std::filesystem::last_write_time(path.std_path().u8string());
    }

    void last_write_time(const b::fs::path& path, std::filesystem::file_time_type newTime) {
        std::filesystem::last_write_time(path.std_path().u8string(), newTime);
    }

    bool remove(const b::fs::path& path) {
        return std::filesystem::remove(path.std_path().u8string());
    }

    size_t remove_all(const b::fs::path& path) {
        return std::filesystem::remove_all(path.std_path().u8string());
    }

} // namespace b::fs