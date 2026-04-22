#!/bin/bash
# 1. Standard Linux Build
mkdir -p build_linux && cd build_linux
cmake .. && make
cd ..

# 2. AppImage Creation
mkdir -p Multiverse.AppDir/usr/bin
cp build_linux/MultiverseApp Multiverse.AppDir/usr/bin/
cp icon.png Multiverse.AppDir/

# Create mandatory AppRun script
cat <<EOF > Multiverse.AppDir/AppRun
#!/bin/sh
HERE="\$(dirname "\$(readlink -f "\${0}")")"
exec "\${HERE}/usr/bin/MultiverseApp"
EOF
chmod +x Multiverse.AppDir/AppRun

# Create mandatory Desktop file
cat <<EOF > Multiverse.AppDir/Multiverse.desktop
[Desktop Entry]
Name=Owen's Multiverse
Exec=MultiverseApp
Icon=icon
Type=Application
Terminal=true
Categories=Game;
EOF

# Build the AppImage
ARCH=x86_64 appimagetool Multiverse.AppDir Multiverse_Linux.AppImage

# 4. Final Packaging
echo "Cleaning old releases..."
rm -rf final_release
mkdir -p final_release

echo "Moving files to release folder..."
cp Multiverse_Linux.AppImage final_release/

