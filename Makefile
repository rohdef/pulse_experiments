rf_pamonitor: pa_mon.cpp
	g++ -o rf_pamonitor pa_mon.cpp -std=c++11 -l pulse -l pulse-simple -l pulse-mainloop-glib
