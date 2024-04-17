#!/bin/bash

# Example command line: ./pull-assignments.sh a2-classroom clone

# Ensure the script is executed from the root of your personal repository
cd /home/reecewayt/ece373/linux-device-drivers

# A list of assignment repositories with their corresponding directory names and remote URLs
# Format is "dirName remoteName repoUrl"
ASSIGNMENTS=(
  "Assignment1 assignment1-classroom git@github.com:ECE-373/assignment-1-hello-linux-reecewayt.git"
  "Assignment2-character-driver a2-classroom git@github.com:ECE-373/assignment-2-character-driver-reecewayt.git"
  # Add more assignments as needed
)

# Function to clone an assignment repository into a specified directory
function clone_assignment() {
  local dirName="$1"
  local remoteName="$2"
  local repoUrl="$3"

  if [ -d "$dirName" ]; then
    echo "Directory $dirName already exists. Skipping clone."
  else
    echo "Cloning $repoUrl into $dirName..."
    git clone "$repoUrl" "$dirName"
    # Set the remote name after cloning
    git -C "$dirName" remote rename origin "$remoteName"
  fi
}

# Function to pull updates for an existing assignment repository
function pull_updates() {
  local dirName="$1"
  local remoteName="$2"

  if [ -d "$dirName" ]; then
    echo "Pulling updates for $dirName from $remoteName..."
    git -C "$dirName" pull "$remoteName" main
  else
    echo "Directory $dirName does not exist. Cannot pull updates."
  fi
}

# Check command arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <remoteName> <operation>"
    echo "operation: clone or pull"
    exit 1
fi

remoteName="$1"
operation="$2"

# Filter assignments to operate only on the specified remote
for assignment in "${ASSIGNMENTS[@]}"; do
  read -ra ADDR <<< "$assignment"
  if [ "${ADDR[1]}" == "$remoteName" ]; then
    case "$operation" in
      clone)
        clone_assignment "${ADDR[0]}" "${ADDR[1]}" "${ADDR[2]}"
        ;;
      pull)
        pull_updates "${ADDR[0]}" "${ADDR[1]}"
        ;;
      *)
        echo "Invalid operation: $operation"
        echo "Valid operations are 'clone' or 'pull'"
        exit 2
        ;;
    esac
  fi
done

echo "Operation $operation completed for $remoteName."
