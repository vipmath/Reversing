add_executable(Reversing main.cc board/Board.cc
            net/helper.cc net/Network.cc mcts/MCTSEngine.cc
            preprocess/Preprocess.cc mcts/TreeNode.cc)
target_link_libraries(Reversing ${Boost_LIBRARIES} mxnet ${CMAKE_THREAD_LIBS_INIT})