project(pneuro)

set(SRC
activafunc.cpp
activafunc.h
gsettings.h
nn1x.h
nnForwardPropagation.h
nnGATraining.h
nnifc.cpp
nnifc.h
nnIncompleteConnected.h
nnlayer.cpp
nnlayer.h
nnTraining.h
)

add_library(${PROJECT_NAME} ${SRC})