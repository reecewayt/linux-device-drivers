#!/bin/bash

# A script to add remotes for GitHub Classroom assignment repositories

# Go to the root directory of your local repository
cd /path/to/your/linux-device-drivers-repo

# Function to add a remote for an assignment
function add_assignment_remote() {
  local remoteName="$1"
  local repoUrl="$2"

  echo "Adding remote ${remoteName} with URL ${repoUrl}"
  git remote add "${remoteName}" "${repoUrl}"
}

# Example usage:
# add_assignment_remote assignment1-classroom git@github.com:ECE-373/assignment-1-hello-linux-reecewayt.git

# Add the remotes for each assignment below using the add_assignment_remote function
# Uncomment and replace the placeholders with actual values

# add_assignment_remote assignment1-classroom <URL-for-assignment-1>
# add_assignment_remote assignment2-classroom <URL-for-assignment-2>
# ... add more as needed ...

# List the current remotes for verification
git remote -v

echo "All remotes have been added."
