#!/bin/bash

# A script to add remotes for GitHub Classroom assignment repositories

# Go to the root directory of your local repository
#cd /path/to/your/linux-device-drivers-repo

# Function to add a remote for an assignment
function add_assignment_remote() {
  local remoteName="$1"
  local repoUrl="$2"

  echo "Adding remote ${remoteName} with URL ${repoUrl}"
  git remote add "${remoteName}" "${repoUrl}"
}

# Check if the correct number of arguments is passed
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <remoteName> <repoUrl>"
    exit 1
fi

# Example usage:
# add_assignment_remote assignment1-classroom git@github.com:ECE-373/assignment-1-hello-linux-reecewayt.git

# Add the remotes for each assignment below using the add_assignment_remote function
add_assignment_remote "$1" "$2"


# List of current remotes, add to the list as assignments get released. This is just used for record keeping
# add_assignment_remote assignment1-classroom git@github.com:ECE-373/assignment-1-hello-linux-reecewayt.git
# add_assignment_remote a2-classroom git@github.com:ECE-373/assignment-2-character-driver-reecewayt.git
#....
# ... add more as needed ...

# List the current remotes for verification
git remote -v

echo "All remotes have been added."
