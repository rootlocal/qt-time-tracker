# Time Tracker

Build:

~~~shell
git clone <REPO>
cd redmine-time-tracker
cmake -S . -B build -G "Ninja" -D CMAKE_BUILD_TYPE=Release -D BUILD_TESTING=OFF
cmake --build build --parallel
# and run 
./build/app/bin/time_tracker
~~~
