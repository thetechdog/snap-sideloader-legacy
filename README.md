# Snap Package Sideloader GUI
A graphical frontend for sideloading .snap packages inspired by  
the likes of GDebi written in C++ and using Qt.   
## Building
Open the project CMakeLists file in Qt Creator, and then build.
## TODO
- Toggle to install snap unconfined
- Show installation progress instead of just alerting the user
- Move install process to another thread to keep the UI responsive  
- Have it be able to open files straight from the file manager  
- Improve labels and text to be more intuitive
- Handle errors better
- Add support for other types of packages (far future)
- Maybe refactor some code as it it messy
- Whatever else I forgot
  
Either way, I'd say the program is usable as is and it does what it's supposed to do.
## Reinstall vs. Remove
Reinstall button wil remove the package and install it again. Useful if you want to upgrade the package.  
Remove button will remove --purge, freeing up space on the disk.
## Credits
(C) Andrei Ionel 2024
## Other
Snap packages documentation: https://snapcraft.io/docs/the-snap-format  
## Tests
Program tested on Ubuntu, Debian and Fedora amd64.
## Screenshots
![Sideloader3](https://github.com/user-attachments/assets/fca5dedd-63e5-495e-b11c-ade17dda5c7a)
![Sideloader2](https://github.com/user-attachments/assets/2fe95c8e-8d5e-477e-bdbc-9494d753ad8e)
![Sideloader1](https://github.com/user-attachments/assets/79dbf7d8-88f6-4fcc-ac8c-93c7048adf57)

EDIT: Project archived, replaced by a new version available at https://github.com/thetechdog/local-snap-sideloader
