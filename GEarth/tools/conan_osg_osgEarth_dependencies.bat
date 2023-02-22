cd %cd%
cd ../conan/release/
conan install . --build=missing --profile=default


cd ../debug/
conan install . --build=missing --profile=debug
