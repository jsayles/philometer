import rhinoscriptsyntax as rs
import random
from System.Drawing import Color

class DataSet:
	def __init__(self):
		self.timestamps = []
		self.markers = []
		self.data = {}
		self.data['brain_connection'] = { 'title':'Brain_Signal', 'color':Color.FromArgb(255,255,255), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_attention'] = { 'title':'Brain_Attention', 'color':Color.FromArgb(100,100,100), 'values':[], 'max':0, 'min':999999999, 'draw':True }
		self.data['brain_meditation'] = { 'title':'Brain_Meditation', 'color':Color.FromArgb(50,50,50), 'values':[], 'max':0, 'min':999999999, 'draw':True }
		self.data['brain_delta'] = { 'title':'Brain_Delta', 'color':Color.FromArgb(166,206,227), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_theta'] = { 'title':'Brain_Theta', 'color':Color.FromArgb(131,120,180), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_alpha_low'] = { 'title':'Brain_Low_Alpha', 'color':Color.FromArgb(178,223,138), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_alpha_high'] = { 'title':'Brain_High_Alpha', 'color':Color.FromArgb(255,255,153), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_beta_low'] = { 'title':'Brain_Low_Beta', 'color':Color.FromArgb(177,89,40), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_beta_high'] = { 'title':'Brain_High_Beta', 'color':Color.FromArgb(253,191,111), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_gamma_low'] = { 'title':'Brain_Low_Gama', 'color':Color.FromArgb(255,127,0), 'values':[], 'max':0, 'min':999999999 }
		self.data['brain_gamma_high'] = { 'title':'Brain_High_Gama', 'color':Color.FromArgb(202,178,214), 'values':[], 'max':0, 'min':999999999 }
		self.data['heart_ms'] = { 'title':'Heart_MS', 'color':Color.FromArgb(106,61,154), 'values':[], 'max':0, 'min':999999999, 'draw':True }
		self.data['heart_bpm'] = { 'title':'Heart_BPM', 'color':Color.FromArgb(251,154,153), 'values':[], 'max':0, 'min':999999999, 'draw':True }
		self.data['gsr'] = { 'title':'GSR', 'values':[], 'color':Color.FromArgb(51,160,44), 'max':0, 'min':999999999, 'draw':True }
		self.data['temp_body'] = { 'title':'Temp_Body', 'color':Color.FromArgb(227,26,28), 'values':[], 'max':0, 'min':999999999, 'draw':True }
		self.size = 0

	def import_data(self, csv_data):
		items = csv_data.strip("()\n\r").split(",")
		if items and len(items) == 18:
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
			self.import_value('heart_ms', int(items[13]))
			self.import_value('heart_bpm', int(items[14]))
			self.import_value('gsr', float(items[15]))
			self.import_value('temp_body', float(items[16]))
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
	
	# Set units to Inches
	rs.UnitSystem(8, True)
	
	# Delete the existing layers?
	
	# Get the bounds for the Bounding Box
	width = rs.GetReal(message="Width", minimum=0, number=6)
	height = rs.GetReal(message="Height", minimum=0, number=1.25)
	margin_perc = rs.GetReal(message="Margin Percentage", minimum=0.01, number=0.01)
	gap_perc = rs.GetReal(message="Gap Percentage", minimum=0.01, number=0.08)
	print "Width: %s, Height: %s" % (width, height)
	print "Margin: %s%s, Gap: %s%s" % (margin_perc, '%', gap_perc, '%')
	
	# Calculate the gap and margin
	margin_value = width * margin_perc
	adjusted_width = width - 2 * margin_value
	gap_value = height * gap_perc
	adjusted_height = height - gap_value
	print "Margin Value: %s, Gap Value: %s" % (margin_value, gap_value)
	print "Adjusted Width: %s, Adjusted Height: %s" % (adjusted_width, adjusted_height)
	
	# Add the bounding box
	#base_layer = rs.AddLayer("Base")
	#rs.CurrentLayer(layer=base_layer)
	#rs.AddRectangle(plane, width, height)

	# Prompt the user for a data file and load the contents
	filename = rs.OpenFileName("Open Data File")
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
		if 'draw' in data and data['draw']:
			data_values = data['values']
			print "%s: size=%s, max=%s, min=%s" % (data['title'], len(data_values), data['max'], data['min'])
			new_layer = rs.AddLayer(data['title'], data['color'])
			rs.CurrentLayer(layer=new_layer)
			graph_points = [[0, 0, 0], [margin_value, gap_value, 0]]
			for i, v in enumerate(data_values):
				x = margin_value + convert_x(i, dataset.size, adjusted_width)
				y = gap_value + convert_y(v, data['max'], data['min'], adjusted_height)
				z = 0
				point = [x, y, z]
				graph_points.append(point)
			graph_points.append([width - margin_value, gap_value, 0])
			graph_points.append([width, 0, 0])
			try:
				new_polyline = rs.AddPolyline(graph_points)
				rs.MirrorObject(new_polyline, [0, 0, 0], [width, 0, 0], copy=True)
			except:
				print "Could not draw: "
				print graph_points
			#rs.AddPlanarSrf(new_polyline)

	# Go back to the base layer
	#rs.CurrentLayer(layer=base_layer)
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
	
