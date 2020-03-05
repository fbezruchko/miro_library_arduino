// miro.js

(function(ext) {
    var device = null;
	
	ext.resetAll = function(){};
	
	ext.runArduino = function(){
		responseValue();
	};
	
	ext.movePWMTime = function(lin_speed,ang_speed,time_ms) {
        runPackage(5,short2array(leftSpeed),short2array(rightSpeed));
    };
    
	function sendPackage(argList, type){
		var bytes = [0xff, 0x55, 0, 0, type];
		bytes[2] = bytes.length - 3;
		device.send(bytes);
	}
	
	function runPackage(){
		sendPackage(arguments, 2);
	}
	
	function getPackage(){
	}
	
    function processData(bytes) {
    }

    var device = null;

    // Extension API interactions
    var potentialDevices = [];
    ext._deviceConnected = function(dev) {
        potentialDevices.push(dev);

        if (!device) {
            tryNextDevice();
        }
    }

    function tryNextDevice() {
        // If potentialDevices is empty, device will be undefined.
        // That will get us back here next time a device is connected.
        device = potentialDevices.shift();
        if (device) {
            device.open({ stopBits: 0, bitRate: 115200, ctsFlowControl: 0 }, deviceOpened);
        }
    }

    function deviceOpened(dev) {
        if (!dev) {
            // Opening the port failed.
            tryNextDevice();
            return;
        }
        device.set_receive_handler('Miro',function(data) {
            processData(data);
        });
    };

    ext._deviceRemoved = function(dev) {
        if(device != dev) return;
        device = null;
    };

    ext._shutdown = function() {
        if(device) device.close();
        device = null;
    };

    ext._getStatus = function() {
        if(!device) return {status: 1, msg: 'Miro disconnected'};
        return {status: 2, msg: 'Miro connected'};
    }

    var descriptor = {};
    ScratchExtensions.register('Miro', descriptor, ext, {type: 'serial'});
})({});
