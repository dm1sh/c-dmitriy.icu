If you dualboot Windows and Linux, you may need to transfer files between your bare-metal Linux and WSL.

Firstly, mount your Windows disk C: on Linux. For it, run:

```
sudo pacman -S ntfs-3g
sudo mkdir -p /mnt/c
sudo mount /dev/<Windows partition name> /mnt/c
```

WSL filesystem is stored as .vhdx file, so we will use qemu-nbd to mount it as usual disk drive.

Install and enable nbd kernel module:

```
sudo pacman -S nbd
sudo modprobe nbd
```

Install qemu:

```
sudo pacman -S qemu
```

Connect .vhdx file to nbd device:

```
sudo qemu-hbd -c /dev/nbd0 /mnt/c/<path to .vhdx file>
```

By default, path must look like Users/[user]/AppData/Local/Packages/[distro]/LocalState/[distroPackageName]/ext4.vhdx

Finally, mount ndb device:

```
sudo mkdir /mnt/wsl
sudo mount /dev/ndb0 /mnt/wsl
```

Now you can transfer files or chroot into it.