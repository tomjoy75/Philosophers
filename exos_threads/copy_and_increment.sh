#!/bin/bash
# Copy and increment a file
folder="."
base_name="code_vault"
extension=".c"
last_file=$(ls $folder/$base_name*$extension | sort -V | tail -n 1)
echo $last_file
