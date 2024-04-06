# vbox-setup.md

This purpose of this document is to outline the steps taken to connect VS Code to a Virtual Machine (via SSH)

1. Run the follow commands on the target VM
```bash
sudo apt-get install openssh-server
sudo service ssh start
sudo systemctl status ssh
#you should see it's now active
ip addr
#write down to loop back IP address
```

2. Install "Remote - SSH" extension in VS Code

3. Edit the `.ssh/config` file in VS Code to include your target VM. 

4. In the VM VirtualBox Manager Window follow this [tutorial](https://dev.to/developertharun/easy-way-to-ssh-into-virtualbox-machine-any-os-just-x-steps-5d9i)

5. From now on you can start the VM with a headless setup
