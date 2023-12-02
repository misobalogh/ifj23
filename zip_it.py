import os
import zipfile
import glob

directory_path = "."
destination = "."
zip_file_name = "xbalog06.zip"

files_to_zip = [
    "*.c",
    "*.h",
    "rozdeleni",
    "Makefile",
    "dokumentace.pdf",
]

os.chdir(directory_path)

with zipfile.ZipFile(os.path.join(destination, zip_file_name), "w") as zipf:
    for pattern in files_to_zip:
        for file in glob.glob(pattern):
            zipf.write(file)

print(f"Zip file '{zip_file_name}' created successfully.")
