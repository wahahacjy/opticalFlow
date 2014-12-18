VPATH = include
OBJDIR = objects
BIN = bin
source = OP.cpp LK.cpp AnomalyDetect.cpp
objects = $(source:%.cpp=$(OBJDIR)/%.o)

all:OPDetect PreOP
.PHONY: all

OPDetect: OPDetect.cpp $(objects)
	@mkdir -p $(BIN) 
	g++ -g3 -o $(BIN)/$@ $^ `pkg-config opencv --libs`

$(objects) : $(OBJDIR)/%.o: %.cpp %.h
	@mkdir -p $(OBJDIR)
	g++ -g3 -c $< -o $@

PreOP: PreOP.cpp $(objects)
	@mkdir -p $(BIN)
	g++ -g3 -o $(BIN)/$@ $^ `pkg-config opencv --libs`

clean:
	rm -f $(BIN)/* $(OBJDIR)/* 
