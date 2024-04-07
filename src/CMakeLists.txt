
add_executable(IsingModel WIN32
               MainApp.cpp 
               MainApp.hpp 
               MainFrame.cpp 
               MainFrame.hpp 
               BufferedBitmap.cpp
               BufferedBitmap.hpp
               Ising.cpp
               Ising.hpp
               wxChartCtrl.cpp 
               wxChartCtrl.hpp
               )

target_link_libraries(IsingModel PRIVATE external_deps)