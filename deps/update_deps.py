import os
import shutil
import requests
import tarfile

github_dependencies = [
    # [ "ocornut", "imgui", "v1.89.8" ],
    # [ "fmtlib", "fmt", "10.0.0" ],                    # MIT-alike License
    [ "gabime", "spdlog", "v1.12.0" ],                 # MIT License
    [ "DaanDeMeyer", "reproc", "v14.2.4" ],            # MIT License
    [ "nemtrif", "utfcpp", "v3.2.4" ],                 # Boost Software License 1.0
    [ "jothepro", "doxygen-awesome-css", "v2.2.1" ],   # MIT License
    [ "batterycenter", "embed", "v1.0.0" ],            # Apache-2.0 License
]

def main():
    deps_folder = os.path.dirname(os.path.realpath(__file__))

    # First remove all folders except the script itself
    for file in os.listdir(deps_folder):
        if os.path.isdir(file):
            print(f"Removing {file}...")
            shutil.rmtree(file)
            print(f"Removing {file}... Done")

    for dep in github_dependencies:
        tmpfolder = os.path.join(deps_folder, "tmp")
        url = f'https://github.com/{dep[0]}/{dep[1]}/archive/refs/tags/{dep[2]}.tar.gz'

        print(f"Downloading and extracting {dep[0]}/{dep[1]}@{dep[2]} from GitHub...")
        response = requests.get(url, stream=True)
        if (response.status_code != 200):
            raise Exception(f"Failed to download {url}")

        file = tarfile.open(fileobj=response.raw, mode="r|gz")

        # With GitHub, the archive contains a folder with the name and version (like imgui-1.89.8)
        # We want the files to live in a folder called just "imgui", so we have to rename it on-disk,
        # as modifying the tarfile before extracting it would involve writing it to disk anyways.
        file.extractall(tmpfolder)
        dir = os.listdir(tmpfolder)
        if len(dir) != 1:
            raise Exception("Expected a single folder in the archive, found more")
        os.rename(os.path.join(tmpfolder, dir[0]), os.path.join(deps_folder, dep[1]))
        shutil.rmtree(tmpfolder)

        print(f"Downloading and extracting {dep[0]}/{dep[1]}@{dep[2]} from GitHub... Done")

if __name__ == "__main__":
    main()