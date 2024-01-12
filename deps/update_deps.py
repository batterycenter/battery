import os
import shutil
import requests
import zipfile

github_dependencies = [
    # [ "fmtlib", "fmt", "10.0.0" ],                   # MIT-alike License
    [ "ocornut", "imgui", "v1.89.8" ],                 # MIT License
    [ "gabime", "spdlog", "v1.12.0" ],                 # MIT License
    [ "DaanDeMeyer", "reproc", "v14.2.4" ],            # MIT License
    [ "nemtrif", "utfcpp", "v3.2.4" ],                 # Boost Software License 1.0
    [ "jothepro", "doxygen-awesome-css", "v2.2.1" ],   # MIT License
    [ "batterycenter", "embed", "v1.2.8" ],            # Apache-2.0 License
    [ "libsdl-org", "SDL", "release-2.28.5" ],         # Zlib License
    [ "libsdl-org", "SDL_image", "release-2.6.3" ],    # Zlib License
    [ "gelldur", "EventBus", "v3.0.4" ],               # Apache-2.0 License
    [ "kunitoki", "LuaBridge3", "3.0-rc4" ],           # MIT License
    [ "walterschell", "Lua", "v5.4.5" ],               # No License
    # [ "CLIUtils", "CLI11", "v2.3.2" ],               # 3-Clause BSD License
]

def main():
    deps_folder = os.path.dirname(os.path.realpath(__file__))

    # First remove all folders except the script itself
    for file in os.listdir(deps_folder):
        if os.path.isdir(file):
            print(f"Removing {file}...")
            shutil.rmtree(file)
            print(f"Removing {file}... Done")

    # Download and extract all repositories from GitHub
    for dep in github_dependencies:
        tmpfolder = os.path.join(deps_folder, "tmp")
        extractfolder = os.path.join(tmpfolder, "extracted")
        url = f'https://github.com/{dep[0]}/{dep[1]}/archive/refs/tags/{dep[2]}.zip'

        print(f"Downloading and extracting {dep[0]}/{dep[1]}@{dep[2]} from GitHub...")
        response = requests.get(url, stream=True)
        if (response.status_code != 200):
            raise Exception(f"Failed to download {url}")

        os.makedirs(tmpfolder, exist_ok=True)
        with open("tmp/archive.zip", "wb") as file:
            for chunk in response.iter_content(chunk_size=1024):
                file.write(chunk)

        zip = zipfile.ZipFile("tmp/archive.zip", "r")
        zip.extractall(extractfolder)
        zip.close()

        # With GitHub, the archive contains a folder with the name and version (like imgui-1.89.8)
        # We want the files to live in a folder called just "imgui", so we have to rename it on-disk,
        # as modifying the tarfile before extracting it would involve writing it to disk anyways.

        dir = os.listdir(extractfolder)
        if len(dir) != 1:
            raise Exception("Expected a single folder in the archive, found more")
        os.rename(os.path.join(extractfolder, dir[0]), os.path.join(deps_folder, dep[1]))
        shutil.rmtree(tmpfolder)

        print(f"Downloading and extracting {dep[0]}/{dep[1]}@{dep[2]} from GitHub... Done")

    # And now delete certain files
    print("Deleting SDL2 unit test files...")       # Delete test folder because it is huge and not necessary for us
    shutil.rmtree(os.path.join(deps_folder, "SDL", "test"))
    print("Deleting SDL2 unit test files... Done")

    # Patch EventBus
    print("Patching EventBus...")
    with open(os.path.join(deps_folder, "EventBus", "CMakeLists.txt"), "r") as file:
        content = file.read()
        content = content.replace("add_subdirectory(use_case)", "#add_subdirectory(use_case)")
        with open(os.path.join(deps_folder, "EventBus", "CMakeLists.txt"), "w") as file:
            file.write(content)
    print("Patching EventBus... Done")

if __name__ == "__main__":
    main()
