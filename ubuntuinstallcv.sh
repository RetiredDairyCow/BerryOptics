# !/bin/bash
   echo y | sudo apt-get update
   echo y | sudo apt-get upgrade

   echo y | sudo apt-get install build-essential cmake git pkg-config

   echo y | sudo apt-get install libjpeg8-dev libtiff4-dev libjasper-dev libpng12-dev

   echo y | sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
   echo y | sudo apt-get install libxvidcore-dev libx264-dev

   echo y | sudo apt-get install libgtk-3-dev

   echo y | sudo apt-get install libatlas-base-dev gfortran

   echo y | sudo apt-get install python2.7-dev python3.5-dev

   cd ~
   wget -O opencv.zip https://github.com/Itseez/opencv/archive/3.1.0.zip
   unzip opencv.zip

   wget -O opencv_contrib.zip https://github.com/Itseez/opencv_contrib/archive/3.1.0.zip
   unzip opencv_contrib.zip

   cd ~
   wget https://bootstrap.pypa.io/get-pip.py
   sudo python get-pip.py

   echo y | sudo pip install virtualenv virtualenvwrapper
   sudo rm -rf ~/.cache/pip

   export WORKON_HOME=$HOME/.virtualenvs
   source /usr/local/bin/virtualenvwrapper.sh

   echo -e "\n# virtualenv and virtualenvwrapper" >> ~/.bashrc
   echo "export WORKON_HOME=$HOME/.virtualenvs" >> ~/.bashrc
   echo "source /usr/local/bin/virtualenvwrapper.sh" >> ~/.bashrc
   source ~/.bashrc

   mkvirtualenv cv -p python2
   workon cv

   pip install numpy
   workon cv

   cd ~/opencv-3.1.0/
   mkdir build
   cd build
   cmake -D ENABLE_PRECOMPILED_HEADERS=OFF -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D INSTALL_PYTHON_EXAMPLES=ON -D INSTALL_C_EXAMPLES=OFF -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.1.0/modules -D PYTHON_EXECUTABLE=~/.virtualenvs/cv/bin/python -D BUILD_EXAMPLES=ON ..

   cmake
   make clean
   make -j4

   sudo make install
   sudo ldconfig

   cd ~/.virtualenvs/cv/lib/python2.7/site-packages/
   ln -s /usr/local/lib/python2.7/site-packages/cv2.so cv2.so

   workon cv
   python
#  Type
#  >>> import cv2
#  Should display
#  >>> cv2.__version__
# '3.0.0'
