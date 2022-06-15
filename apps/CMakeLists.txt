add_executable(app app.cpp)
add_executable(app2 app2.cpp)

target_link_libraries(app PRIVATE libquadtree)
target_link_libraries(app2 PRIVATE libquadtree)
