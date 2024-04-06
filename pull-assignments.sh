#!/bin/bash

# Ensure the script is executed from the root of your personal repository
cd /home/reecewayt/ece373/linux-device-drivers

# A list of assignment subtrees and their corresponding remote names
# Format is "dirName remoteName branch"
# Example:
# Assignment1 assignment1-classroom main
# Assignment2 assignment2-classroom main
# ...
ASSIGNMENTS=(
  "Assignment1 assignment1-classroom main"
  # TODO add more assignments as needed
)

# Function to pull updates for a single assignment
function pull_updates() {
  local dirName="$1"
  local remoteName="$2"
  local branch="$3"

  echo "Pulling updates for ${dirName} from ${remoteName}/${branch}..."
  git subtree pull --prefix="${dirName}" "${remoteName}" "${branch}" --squash
}

# Iterate over the assignments and pull updates for each one
for assignment in "${ASSIGNMENTS[@]}"; do
  # Split the string into an array
  read -ra ADDR <<< "$assignment"
  # Call the pull_updates function with the split elements
  pull_updates "${ADDR[0]}" "${ADDR[1]}" "${ADDR[2]}"
done

echo "All updates have been pulled."
