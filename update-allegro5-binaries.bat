@echo off

cd "%~dp0"
cd modules\allegro5-binaries
echo Pulling from github.com ...
git restore *
git pull origin master
echo Done

Pause