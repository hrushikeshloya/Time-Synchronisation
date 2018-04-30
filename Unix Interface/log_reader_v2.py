## Time synchronization

from scipy.signal import butter, lfilter, freqz
import pylab as plt
import numpy as np
from scipy import signal

def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y


## Low pass filter specs
order = 6
fs = 2500.0       # sample rate, Hz
cutoff = 25.0  # desired cutoff frequency of the filter, Hz

# Get the filter coefficients so we can check its frequency response.
b, a = butter_lowpass(cutoff, fs, order)


f = open('mode1-ex8')  ### change this according to your log file name
# f_arr = f.read().split('\n')[9:-21]
# f_arr = f.read().split('\n')[20:-21] ##for ex4 
# f_arr = f.read().split('\n')[22:-21]  ##for ex5

f_arr = f.read().split('\n')[17:-21] ##for ex6,7,8


#### With timesync analysis ####

timestamps = []
for i in range(18,len(f_arr),21):
	##Timesync repeats after 21 samples
	timestamps.append(int(f_arr[i].replace('\r','')))



ts1_x = []
ts1_y = []
ts1_z = []

ts2_x = []
ts2_y = []
ts2_z = []



timestamps_locations_s1 = []
timestamps_locations_s2 = []
s1_count = 0
s2_count = 0
iterations = 0
for i in f_arr:
	iterations += 1
	if(iterations <= 5):
		if "s1" in i:
			if "&&&" in i:
				timestamps_locations_s1.append(s1_count+i.find('&&&')-2)
			fs1 = i[2:-6].replace('&&&','')
			for j in range(len(fs1)):
				s1_count += 1
				if(j%3 == 0):
					ts1_x.append(ord(fs1[j]))
				elif(j%3 == 1 ):
					ts1_y.append(ord(fs1[j]))
				else:
					ts1_z.append(ord(fs1[j]))
		elif "s2" in i:
			if "&&&" in i:
				timestamps_locations_s2.append(s2_count+i.find('&&&')-2)
			fs2 = i[2:-6].replace('&&&','')
			#print fs2
			#print offset
			for j in range(len(fs2)):
				s2_count += 1
				if(j%3 == 0):
					ts2_x.append(ord(fs2[j]))
				elif(j%3 == 1 ):
					ts2_y.append(ord(fs2[j]))
				else:
					ts2_z.append(ord(fs2[j]))
	### Offset calculation
	if(iterations == 5):
		fs1_x_offset = np.mean(ts1_x) 
		fs1_y_offset = np.mean(ts1_y)
		fs1_z_offset = np.mean(ts1_z)
		fs2_x_offset = np.mean(ts2_x)
		fs2_y_offset = np.mean(ts2_y)
		fs2_z_offset = np.mean(ts2_z)
	if(iterations > 5):
		if "s1" in i:
			if "&&&" in i:
				timestamps_locations_s1.append(s1_count+i.find('&&&')-2)
			fs1 = i[2:-6].replace('&&&','')
			# print fs1
			# print offset
			for j in range(len(fs1)):
				s1_count += 1
				if(j%3 == 0):
					ts1_x.append(10*(ord(fs1[j])-fs1_x_offset))
				elif(j%3 == 1 ):
					ts1_y.append(10*(ord(fs1[j])-fs1_y_offset))
				else:
					ts1_z.append(10*(ord(fs1[j])-fs1_z_offset))
		elif "s2" in i:
			if "&&&" in i:
				timestamps_locations_s2.append(s2_count+i.find('&&&')-2)
			fs2 = i[2:-6].replace('&&&','')
			#print fs2
			#print offset
			for j in range(len(fs2)):
				s2_count += 1
				if(j%3 == 0):
					ts2_x.append(10*(ord(fs2[j])-fs2_x_offset))
				elif(j%3 == 1 ):
					ts2_y.append(10*(ord(fs2[j])-fs2_y_offset))
				else:
					ts2_z.append(10*(ord(fs2[j])-fs2_z_offset))

ts1_x_filtered = butter_lowpass_filter(ts1_x, cutoff, fs, order)
ts1_y_filtered = butter_lowpass_filter(ts1_y, cutoff, fs, order)
ts1_z_filtered = butter_lowpass_filter(ts1_z, cutoff, fs, order)

ts2_x_filtered = butter_lowpass_filter(ts2_x, cutoff, fs, order)
ts2_y_filtered = butter_lowpass_filter(ts2_y, cutoff, fs, order)
ts2_z_filtered = butter_lowpass_filter(ts2_z, cutoff, fs, order)


timestamps_locations_s1 = np.array(timestamps_locations_s1)/3
timestamps_locations_s2 = np.array(timestamps_locations_s2)/3



count = 0
T_s2 = np.zeros(len(ts2_z))
for i in range(len(T_s2)):
	if i in timestamps_locations_s2:
		if(count < len(timestamps)):
			T_s2[i] = timestamps[count]
		count += 1
	else:
		T_s2[i] = T_s2[i-1] + 400

T_s2_uniform = np.zeros(len(ts2_z))
for i in range(len(T_s2)):
	T_s2_uniform[i] = T_s2_uniform[i-1] + 400


count = 0
T_s1 = np.zeros(len(ts1_z))
for i in range(len(T_s1)):
	if i in timestamps_locations_s1:
		if(count < len(timestamps)):
			T_s1[i] = timestamps[count]
		count += 1
	else:
		T_s1[i] = T_s1[i-1] + 400

T_s1_uniform = np.zeros(len(ts1_z))
for i in range(len(T_s1)):
	T_s1_uniform[i] = T_s1_uniform[i-1] + 400



plt.plot(T_s2_uniform, ts2_z_filtered, label = 's2 unsync')
plt.plot(T_s1_uniform, ts1_z_filtered, label = 's1 unsync')
plt.legend()
plt.show()


plt.plot(T_s2, ts2_z_filtered, label = 's2 sync')
plt.plot(T_s1, ts1_z_filtered, label = 's1 sync')
plt.legend()
plt.show()
