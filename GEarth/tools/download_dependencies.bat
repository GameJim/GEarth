set BUILD_FOLDER=%cd%

REM Disable the warning for detached head
git config --global advice.detachedHead false

git clone https://github.com/Tencent/rapidjson.git ../third_party/rapidjson

cd %BUILD_FOLDER%
curl -o ../third_party/third_party.zip http://47.100.76.140:3337/index.php/apps/files/ajax/download.php?dir=%2F%E4%BB%A3%E7%A0%81%2FC%2B%2B%2F3rdParty%2FGEarth&files=third_party.zip&downloadStartSecret=xah443le8u