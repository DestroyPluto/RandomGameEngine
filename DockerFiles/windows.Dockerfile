# Use the latest Windows Server Core image with .NET Framework 4.8.
FROM mcr.microsoft.com/windows/servercore:ltsc2019

# Restore the default Windows shell for correct batch processing.
SHELL ["cmd", "/S", "/C"]

# Download the Build Tools bootstrapper outside of the PATH.
ADD https://aka.ms/vs/16/release/vs_buildtools.exe C:\\TEMP\\vs_buildtools.exe

# Install Visual Studio Build Tools
RUN C:\\TEMP\\vs_buildtools.exe --quiet --wait --norestart --nocache \
    --installPath C:\\BuildTools \
    --add Microsoft.VisualStudio.Workload.VCTools \
    --includeRecommended

# Install CMake
ADD https://github.com/Kitware/CMake/releases/download/v3.21.1/cmake-3.21.1-win64-x64.msi C:\\TEMP\\cmake-3.21.1-win64-x64.msi
RUN msiexec.exe /i C:\\TEMP\\cmake-3.21.1-win64-x64.msi /quiet /norestart

# Clean the image
RUN del C:\\TEMP\\*
