import rhinoscriptsyntax as rs
import random
from System.Drawing import Color

class DataSet:
	def __init__(self):
		self.timestamps = []
		self.markers = []
		self.data = {}
		self.data['brain_connection'] = { 'title':'Brain_Signal', 'color':Color.FromArgb(255,0,255), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_attention'] = { 'title':'Brain_Attention', 'color':Color.FromArgb(255,0,0), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_meditation'] = { 'title':'Brain_Meditation', 'color':Color.FromArgb(255,255,0), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_delta'] = { 'title':'Brain_Delta', 'color':Color.FromArgb(100,0,0), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_theta'] = { 'title':'Brain_Theta', 'color':Color.FromArgb(100,0,100), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_alpha_low'] = { 'title':'Brain_Low_Alpha', 'color':Color.FromArgb(100,100,0), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_alpha_high'] = { 'title':'Brain_High_Alpha', 'color':Color.FromArgb(100,255,100), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_beta_low'] = { 'title':'Brain_Low_Beta', 'color':Color.FromArgb(100,255,0), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_beta_high'] = { 'title':'Brain_High_Beta', 'color':Color.FromArgb(0,255,100), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_gamma_low'] = { 'title':'Brain_Low_Gama', 'color':Color.FromArgb(0,100,100), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_gamma_high'] = { 'title':'Brain_High_Gama', 'color':Color.FromArgb(0,100,255), 'values':[], 'max':0, 'min':999999999 }
		self.data['heart_freq'] = { 'title':'Heart_Frequency', 'color':Color.FromArgb(0,0,100), 'values':[], 'max':0, 'min':999999999 }
		self.data['heart_bpm'] = { 'title':'Heart_BPM', 'color':Color.FromArgb(255,255,100), 'values':[], 'max':0, 'min':999999999 }
		self.data['gsr'] = { 'title':'GSR', 'values':[], 'color':Color.FromArgb(0,0,255), 'max':0, 'min':999999999 }
		self.data['temp_body'] = { 'title':'Temp_Body', 'color':Color.FromArgb(0,255,0), 'values':[], 'max':0, 'min':999999999 }
		#self.data['temp_room'] = { 'title':'Temp_Room', 'color':Color.FromArgb(255,100,255), 'values':[], 'max':0, 'min':999999999 }
		self.data['sound_level'] = { 'title':'Sound_Level', 'color':Color.FromArgb(255,100,100), 'values':[], 'max':0, 'min':999999999 }
		self.size = 0

	def import_data(self, csv_data):
		items = csv_data.strip("()\n\r").split(",")
		if items and len(items) == 20:
			self.timestamps.append(items[0])
			self.markers.append(int(items[1]))
			self.import_value('brain_connection', int(items[2]))
			self.import_value('brain_attention', int(items[3]))
			self.import_value('brain_meditation', int(items[4]))
			self.import_value('brain_delta', int(items[5]))
			self.import_value('brain_theta', int(items[6]))
			self.import_value('brain_alpha_low', int(items[7]))
			self.import_value('brain_alpha_high', int(items[8]))
			self.import_value('brain_beta_low', int(items[9]))
			self.import_value('brain_beta_high', int(items[10]))
			self.import_value('brain_gamma_low', int(items[11]))
			self.import_value('brain_gamma_high', int(items[12]))
			self.import_value('heart_freq', int(items[13]))
			self.import_value('heart_bpm', int(items[14]))
			self.import_value('gsr', float(items[15]))
			self.import_value('temp_body', float(items[16]))
			#self.import_value('temp_room', float(items[17]))
			self.import_value('sound_level', int(items[19]))
			self.size = self.size + 1
	
	def import_value(self, key, val):
		dict = self.data[key]
		dict['values'].append(val)
		if val > dict['max']:
			dict['max'] = val
		if val < dict['min']:
			dict['min'] = val

def main():
	plane = rs.WorldXYPlane()
	
	# Get the bounds for the Bounding Box
	#width = rs.GetInteger(message="Width?", minimum=0)
	#height = rs.GetInteger(message="Height?", minimum=0)
	width=30
	height=15
	
	# Add the bounding box
	base_layer = rs.AddLayer("Base")
	rs.CurrentLayer(layer=base_layer)
	rs.AddRectangle(plane, width, height)

	# Prompt the user for a data file and load the contents
	#filename = rs.OpenFileName("Open Data File")
	filename = "testData.csv"
	if not filename: return
	print "Loading %s" % filename
	file = open(filename, "r")
	file_contents = file.readlines()
	file.close()

	# Parse the file contents into our organized data set
	dataset = DataSet()
	for line in file_contents:
		dataset.import_data(line)
	print "Data Set Size: %s" % dataset.size
	
	# Go through each set of data and draw the graph
	for key in dataset.data.keys():
		data = dataset.data[key]
		data_values = data['values']
		print "%s: size=%s, max=%s, min=%s" % (data['title'], len(data_values), data['max'], data['min'])
		new_layer = rs.AddLayer(data['title'], data['color'])
		rs.CurrentLayer(layer=new_layer)
		graph_points = [[width, 0, 0], [0, 0, 0]]
		for i, v in enumerate(data_values):
			x = convert_x(i, dataset.size, width)
			y = convert_y(v, data['max'], data['min'], height)
			z = 0
			point = [x, y, z]
			graph_points.append(point)
		graph_points.append([width, 0, 0])
		new_polyline = rs.AddPolyline(graph_points)
		#rs.AddPlanarSrf(new_polyline)

	# Go back to the base layer
	rs.CurrentLayer(layer=base_layer)
	print "Done!"
	print

def convert_x(value, rows, width):
	ratio = float(width) / float(rows)
	return float(value) * ratio

def convert_y(value, max_val, min_val, height):
	adj_max = float(max_val) * 1.02
	adj_min = min_val
	#if min_val == 0:
	#	adj_min = adj_max * 0.1
	#	adj_max = adj_max + adj_min
	if value == 0:
		#return adj_min
		return 0.1
	ratio = float(height) / adj_max
	return float(value) * ratio

def randomColor():
	red = int(255*random.random())
	green = int(255*random.random())
	blue = int(255*random.random())
	return Color.FromArgb(red,green,blue)

if( __name__ == "__main__" ):
	main()
	
