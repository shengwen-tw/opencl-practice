## OpenCL 2.1 Installation (Intel)

The following commands have been tested on my laptop with Intel i5-6200 CPU and integrated HD Graphics 520 GPU.

The newest version of intel compute runtime library failed to work on my machine, therefore i'm using the older version released in 2019.

```
#install shared objects
mkdir -p ~/intel && cd ~/intel
wget https://github.com/intel/compute-runtime/releases/download/19.07.12410/intel-gmmlib_18.4.1_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/19.07.12410/intel-igc-core_18.50.1270_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/19.07.12410/intel-igc-opencl_18.50.1270_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/19.07.12410/intel-opencl_19.07.12410_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/19.07.12410/intel-ocloc_19.07.12410_amd64.deb
sudo dpkg -i *.deb
sudo ln -s /usr/lib/x86_64-linux-gnu/libOpenCL.so.1 /usr/lib/libOpenCL.so

#install headers
git clone https://github.com/KhronosGroup/OpenCL-Headers.git
sudo cp OpenCL-Headers/CL/*.h /usr/include/CL
```

You can use ``clinfo`` command to check if the installation succeeded.

```
sudo apt install clinfo
```
