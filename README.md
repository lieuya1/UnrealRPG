To clone on a new device

Clone the repo
bash   git clone https://github.com/YourUsername/YourRepoName.git

Install Unreal 5.4.4
https://github.com/EpicGames/UnrealEngine
Error 404 - follow this https://www.unrealengine.com/en-US/ue-on-github

Regenerate project files — since Binaries/ and Intermediate/ aren't pushed, you need to right-click the .uproject file → Generate Visual Studio Project Files
Rebuild the project — open the solution in Visual Studio and build it before launching


Possible issues to watch out for

Plugins — if you use any third party plugins, you'll need to reinstall them on the new machine
Git LFS — if you set up LFS, make sure it's installed on the new computer too before pulling, otherwise assets may not download correctly
Engine path differences — if you have any hardcoded paths in your project they may break on a different machine

TO work with your aws GameLiftServer be sure to package it add New folder named PackagedGame on your UNREAL file, then Package the Client first and Package the Server
