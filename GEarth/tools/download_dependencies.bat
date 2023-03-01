set BUILD_FOLDER=%cd%

REM Disable the warning for detached head
git config --global advice.detachedHead false

git clone https://github.com/Tencent/rapidjson.git ../third_party/rapidjson

cd %BUILD_FOLDER%
curl -o ../third_party/third_party.zip http://47.100.76.140:3337/s/dTrWmWEbKCoEAos/download/third_party.zip