# 1. Atomic Motherboards (go boom)   
1.  What is the audio device?  
Realtek ALC888  
2. What device is the GPIO connected to?   
Super IO Winbond W83627DHG      
3.  how many network (LAN) devices are on the motherboard and what are they?   
2 -> Intel 82567V & Intel 82583V  
4.  how many total serial ports does the box support, inside and out?   
9 

# Network Noodling
1. What pins control the LEDs?  
Pins 30, 31, 33
<img src="../image.png" alt="64-Pin QFN Package" width="300" height="300"/>
2. What address offset is the Device Control Register?  
0x00000 or 0x00004
3. What bit in the Device Control Register will force a reset of the network chip?  
Bit 26

# Winken, Blinken, and Nod
1. What register (name and address) controls the LEDs?   
`LEDCTL` register at offset `0x00E00`
2. What bit pattern should you use to turn off LED1?   
`0xF00`
3. What bit pattern should you use to make LED2 blink?  
`0x0080_0000`
<br>
<br>
----------------------------
# Alex -> __IGNORE__

# Notes on PCI Resource Memory Map: 
Resource map of a device can be found at `/sys/bus/pci/devices/<device>/resource`

__Example:__ Intel Ethernet Device
```bash
sudo cat /sys/bus/pci/devices/0000:00:03.0/resource

0xfeae0000 0xfeafffff 0x00000000 #Mem resource
0x00002000 0x0000203f 0x00000001 #I/O Port resource
0xfeac0000 0xfeadffff 0x00000000 #Memory resource

# <start address> <end address> <flags>
```

- Flags: 
    - `0x00000000`: Memory resource (non-prefetchable)
    - `0x00000001`: I/O port resource
    - `0x00000002`: Memory resource (prefetchable)
    - `0x00000004`: Expansion ROM


# Notes on binding and unbinding a device from a PCI Driver
The ethernet device will already be bound to a driver so you will need to unbind then bind the device to your custom driver: 
```bash
#steps
lspci -k -s 00:03.0
#you should get this output
00:03.0 Ethernet controller: Intel Corporation 82540EM Gigabit Ethernet Controller (rev 02)
        Subsystem: Intel Corporation PRO/1000 MT Desktop Adapter
        Kernel driver in use: e1000
        Kernel modules: e1000

#unbind driver
sudo sh -c 'echo 0000:00:03.0 > /sys/bus/pci/drivers/e1000/unbind'


#instantiate your module
sudo insmod e1000

#the probe function should then bind the device to the driver automatically
#to manually bind you can do this command
echo 0000:00:03.0 > /sys/bus/pci/drivers/my_e1000/bind

```
# Get the Vendor and Device ID Dynamically
Below is the method to use to get the vendor and device from the config space of the pci device
```c
pci_read_config_word(dev, 0x0, &vid);
pci_read_config_word(dev, 0x1, &dev_id);
```
