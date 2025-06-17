# Linux Kernel Module Examples

This folder contains examples of Linux kernel module programming, including basic modules and more advanced examples such as character devices, file operations, and device file creation.

## Folder Structure

- `kernel_module_basic/`
  - `hello_kernel/`: Simple "Hello World" kernel module example.
  - `character_device/`: Example of a character device driver, including device number allocation (major/minor), device registration and unregistration.
  - `device_file/`: Example of creating a device file in `/dev` using class and device.
  - `file_operations/`: Example of file operations (open, read, write, release) on a character device, kernel memory management, and user space interaction.

## Build & Install Instructions

1. Navigate to the desired module folder, for example:
    ```sh
    cd kernel_module_basic/hello_kernel
    ```

2. Build the module:
    ```sh
    make
    ```

3. Insert the module into the kernel:
    ```sh
    sudo insmod <module_name>.ko
    ```

4. Check kernel logs:
    ```sh
    dmesg | tail
    ```

5. Remove the module from the kernel:
    ```sh
    sudo rmmod <module_name>
    ```

6. Clean build files:
    ```sh
    make clean
    ```

## Requirements

- Linux operating system
- Kernel build tools (`build-essential`, `linux-headers-$(uname -r)`)

## Author

- huysn11b@gmail.com

---

These examples help you get started with writing, building, and working with Linux kernel modules, as well as communicating between kernel space and user space