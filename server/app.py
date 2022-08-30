from flask import Flask, render_template
from flask_mqtt import Mqtt


# Initializes the Flask app.
app = Flask(__name__)
# Initializes the component to interact with the MQTT broker.
mqttc = Mqtt(app)

# Dictionary with the sensors, actuators and the corresponding data.
sensors = {
	'led': {'board': 'NodeMCU1', 'topic': 'led', 'status': 'off'},
	'temperature': {'board': 'NodeMCU1', 'topic': 'temperature', 'status': ''},
	'ldr': {'board': 'NodeMCU1', 'topic': 'ldr', 'status': ''},
	'button': {'board': 'NodeMCU1', 'topic': 'button', 'status': 0 },
    'sonar': {'board': 'NodeMCU2', 'topic': 'sonar', 'status': ''},
    'servo': {'board': 'NodeMCU2', 'topic': 'servo', 'status': ''}
}

# Grouped to be used by the HTML file.
data = {
    'sensors': sensors,
    'local_ip_1': '',
    'local_ip_2': '',
    'mac_address_1': '',
    'mac_address_2': ''
}

# This function executes when the web server client receives a message from the MQTT broker.
@mqttc.on_message()
def on_message(client, userdata, message):
	if (message.topic == 'value_temperature'):
		sensors['temperature']['status'] = message.payload
	elif (message.topic == 'value_luminosity'):
		sensors['ldr']['status'] = message.payload
	elif (message.topic == 'button'):
		sensors['button']['status'] = sensors['button']['status'] + 1
		if (sensors['button']['status'] == 10):
			sensors['button']['status'] = 0
	elif (message.topic == 'value_distance'):
		sensors['sonar']['status'] = message.payload
	elif (message.topic == 'local_ip_1'):
		data['local_ip_1'] = message.payload
	elif (message.topic == 'local_ip_2'):
		data['local_ip_2'] = message.payload
	elif (message.topic == 'mac_address_1'):
		data['mac_address_1'] = message.payload
	elif (message.topic == 'mac_address_2'):
		data['mac_address_2'] = message.payload
	return render_template('index.html', **data)

# This function executes then the web server connects successfully for the first time to the MQTT broker.
# The web server subscribes to some topics.
@mqttc.on_connect()
def on_connect(client, userdata, flags, rc):
	mqttc.subscribe('value_temperature')
	mqttc.subscribe('value_luminosity')
	mqttc.subscribe('button')
	mqttc.subscribe('value_distance')
	mqttc.subscribe('local_ip_1')
	mqttc.subscribe('local_ip_2')
	mqttc.subscribe('mac_address_1')
	mqttc.subscribe('mac_address_2')

# The web server returns the HTML file with the measured values by the sensors.
@app.route('/')
def main():
	return render_template('index.html', **data)

# Executed when the user clicks on a button of the web page.
# Publishes a message in order to receive data from the NodeMCU modules.
@app.route('/<sensor>/<action>')
def actions(sensor, action):
	# Request local IP and MAC addresses of the NodeMCU modules.
	if data['local_ip_1'] == '':
		mqttc.publish('send_local_ip_1', '')
	if data['local_ip_2'] == '':
		mqttc.publish('send_local_ip_2', '')
	if data['mac_address_1'] == '':
		mqttc.publish('send_mac_address_1', '')
	if data['mac_address_2'] == '':
		mqttc.publish('send_mac_address_2', '')
	# Request to receive info of a specific sensor or perform some action over an actuator.
	mqttc.publish(sensors[sensor]['topic'], action)
	return render_template('index.html', **data)

	
if __name__ == '__main__':
	# Web server runs over localhost on port 8181.
	app.run(host='0.0.0.0', port=8181, debug=True)
	mqttc.init_app(app)


