// Miro.js

(function (ext) {
	var device = null;
	var _rxBuf = [];

	var extID;
	var freeRam;

	var _state = 1;

	var states = {
		closedState: 0,
		loginState: 1,
		passState1: 2,
		passState2: 3,
		workState: 4,
		attachDeviceState: 5,
		detachDeviceState: 6,
		getDeviceIndexState: 7,
		getDeviceNameState: 8,
		setParameterOfDeviceState: 9,
		getParameterOfDeviceState: 10,
		getFreeRamState: 11,
		getValueState: 12
	}

	var valValue = 0;
	var valDevName = "";

	ext.resetAll = function () { };

	ext.runArduino = function () { };

	ext.attachDevice = function (Name) {
		_state = states.attachDeviceState;
		sendString("miroattach " + Name);
	}

	ext.detachDevice = function (index) {
		_state = states.detachDeviceState;
		sendString("mirodetach " + index.toString());
	}

	ext.getDeviceIndex = function (nextID, Name) {
		_state = states.getDeviceIndexState;
		sendString("mirogetdevix " + Name);
	}

	ext.getDeviceName = function (nextID, index) {
		_state = states.getDeviceNameState;
		extID = nextID;
		sendString("mirogetdevname " + index.toString());
	}

	ext.setParameterOfDevice = function (value, parameter, index) {
		_state = states.setParameterOfDeviceState;
		sendString("miroset -d" + " " + index.toString() + " " + parameter.toString() + " " + value.toString());
	}

	ext.getParameterOfDevice = function (nextID, parameter, index) {
		_state = states.getParameterOfDeviceState;
		extID = nextID;
		sendString("miroget -d" + " " + index.toString() + " " + parameter.toString());
	}

	ext.moveLinearAngularSpeed = function (linear_speed, angular_speed) {
		_state = states.moveLinearAngularSpeedState;
		sendString("miroset -m " + linear_speed.toString() + " " + angular_speed.toString());
	}

	ext.moveLinearAngularSpeedDistance = function (linear_speed, angular_speed, distance) {
		_state = states.moveLinearAngularSpeedDistanceState;
		sendString("miroset -m " + linear_speed.toString() + " " + angular_speed.toString() + " " + distance.toString());
	}

	ext.rotateAngularSpeed = function (angular_speed) {
		_state = states.rotateAngularSpeedState;
		sendString("miroset -r " + angular_speed.toString());
	}

	ext.rotateAngularSpeedAngle = function (angular_speed, angle) {
		_state = states.rotateAngularSpeedAngleState;
		sendString("miroset -r " + angular_speed.toString() + " " + angle.toString());
	}

	ext.getFreeRam = function () {
		_state = states.getFreeRamState;
		extID = nextID;
		sendString("");
	}

	function readFloat(arr, position) {
		var f = [arr[position], arr[position + 1], arr[position + 2], arr[position + 3]];
		return parseFloat(f);
	}
	function readInt(arr, position, count) {
		var result = 0;
		for (var i = 0; i < count; ++i) {
			result |= arr[position + i] << (i << 3);
		}
		return result;
	}
	function readDouble(arr, position) {
		return readFloat(arr, position);
	}
	function readString(arr, position, len) {
		var value = "";
		for (var ii = 0; ii < len; ii++) {
			value += String.fromCharCode(arr[ii + position]);
		}
		return value;
	}

	function stateMachine(value) {
		var response = "";

		if (value == "login") _state = states.loginState;
		if (value == "password") {
			if (_state == states.loginState) {
				_state = states.passState1;
			} else {
				_state = states.loginState;
			}
		}

		if (_state == states.loginState) {
			setTimeout(function () { sendString("miro") }, 250);
		} else if (_state == states.passState1) {
			setTimeout(function () { sendString("robot") }, 250);
			_state = states.passState2;
		} else if (_state == states.passState2) {
			_state = states.workState;
		} else if (_state == states.attachDeviceState) {
			_state = states.workState;
			responseValue();
		} else if (_state == states.detachDeviceState) {
			_state = states.workState;
			responseValue();
		} else if (_state == states.getDeviceNameState) {
			if (value.length > 0) {
				_state = states.workState;
				response = value;
				responseValue(extID, response);
			}
		} else if (_state == states.getDeviceIndexState) {
			if (value.length > 0) {
				_state = states.workState;
				response = value;
				responseValue(extID, response);
			}
		} else if (_state == states.setParameterOfDeviceState) {
			_state = states.workState;
			responseValue();
		} else if (_state == states.getParameterOfDeviceState) {
			if (value.length > 0) {
				_state = states.workState;
				response = value;
				responseValue(extID, response);
			}
		} else if (_state == states.moveLinearAngularSpeedState) {
			_state = states.workState;
			responseValue();
		} else if (_state == states.moveLinearAngularSpeedDistanceState) {
			_state = states.workState;
			responseValue();
		} else if (_state == states.rotateAngularSpeedState) {
			_state = states.workState;
			responseValue();
		} else if (_state == states.rotateAngularSpeedAngleState) {
			_state = states.workState;
			responseValue();
		} else if (_state == states.getFreeRamState) {
			_state = states.workState;
			response = freeRam;
		}

		//responseValue();
	}

	function processData(bytes) {
		trace(bytes);
		var sharpParse = 0;
		var endParse = 0;
		var startParse = 0;

		var value = "";
		Array.prototype.push.apply(_rxBuf, bytes);
		var lenBuf = _rxBuf.length;
		while ((lenBuf > 0) && (value == "")) {
			for (var ii = lenBuf - 1; ii > 0; ii--) {
				if (_rxBuf[ii] == 0x3a) {
					if (_rxBuf[ii - 1] == 0x23) {
						// for (var jj = ii - 2; jj >= 5; jj--) {
						// 	if (_rxBuf[jj] == 0x28) {
						// 		freeRam = readString(_rxBuf, jj + 1, ii - 3 - jj); //FreeRAM
						// 		break;
						// 	}
						// }
						sharpParse = ii - 1;
					} else if (readString(_rxBuf, ii - 5, 5) == "login") {
						_rxBuf = [];
						value = "login";
						break;
					} else if (readString(_rxBuf, ii - 8, 8) == "password") {
						_rxBuf = [];
						value = "password";
						break;
					}
				}
			}
			if (sharpParse > 0) {
				for (var ii = sharpParse; ii > 0; ii--) {
					if ((_rxBuf[ii] == 0x0a) && (_rxBuf[ii - 1] == 0x0d)) {
						endParse = ii - 1;
						break;
					} else {
						endParse = 0;
					}
				}
				for (var ii = endParse; ii > 0; ii--) {
					if ((_rxBuf[ii] == 0x0a) && (_rxBuf[ii - 1] == 0x0d)) {
						startParse = ii + 1;
						value = readString(_rxBuf, startParse, endParse - startParse);
						break;
					} else {
						startParse = 0;
					}
				}
				_rxBuf.splice(0, sharpParse + 2);
				lenBuf = _rxBuf.length;
			} else {
				break;
			}
		}

		stateMachine(value);
	}

	// Extension API interactions
	var potentialDevices = [];
	ext._deviceConnected = function (dev) {
		potentialDevices.push(dev);
		if (!device) {
			tryNextDevice();
		}
	}

	function sendString(str) {
		const sending = str + "\n";
		var bytes = [];
		for (var i = 0; i < sending.length; i++) {
			bytes.push(sending[i].charCodeAt());
		}
		device.send(bytes);
	}

	function tryNextDevice() {
		// If potentialDevices is empty, device will be undefined.
		// That will get us back here next time a device is connected.
		device = potentialDevices.shift();
		if (device) {
			_state = states.loginState;
			device.open({ stopBits: 1, bitRate: 115200, ctsFlowControl: 0 }, deviceOpened);
			_rxBuf = [];
		}
	}

	function deviceOpened(dev) {
		if (!dev) {
			// Opening the port failed.
			tryNextDevice();
			return;
		}
		device.set_receive_handler('miro', function (data) {
			processData(data);
		});
	}

	ext._deviceRemoved = function (dev) {
		if (device != dev) return;
		device = null;
	}

	ext._shutdown = function () {
		if (device) device.close();
		device = null;
	}

	ext._getStatus = function () {
		if (!device) return { status: 1, msg: 'miro disconnected' };
		return { status: 2, msg: 'miro connected' };
	}

	var descriptor = {};
	ScratchExtensions.register('miro', descriptor, ext, { type: 'serial' });
})({});
