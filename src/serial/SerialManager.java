package serial;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

import com.fazecast.jSerialComm.SerialPort;

public class SerialManager {
	
	private SerialPort port;
	private String lastPayload;
	private List<String> commLog;
	
	public void openPort(int index) {
		this.port = SerialPort.getCommPorts()[index];
		port.setBaudRate(9600);
		
		commLog = new ArrayList<>();
		
		if (port.openPort()) {
			System.out.println("Successfully opened port at " + port.getDescriptivePortName());
			write(new SerialCommand("HI", 0));
		} else {
			System.err.println("Failed to open port '" + port.getDescriptivePortName() + "'");
		}
	}
	
	public void destroy() {
		port.removeDataListener();
		port.closePort();
		clearCommLog();
		System.out.println("SerialManager was destroyed...");
	}
	
	public List<String> getCommLog() {
		return this.commLog;
	}
	
	public void clearCommLog() {
		this.commLog = new ArrayList<>();
	}
	
	public boolean isReady() {
		return port != null && port.isOpen();
	}
	
	public SerialPort getPort() {
		return this.port;
	}
	
	public void write(SerialCommand sc) {
		if (!isReady())
			return;
		
		if (!sc.isValid())
			return;
		
		String payload = sc.getCommand() + ":" + sc.getValue() + "#";
		try {
			PrintWriter pw = new PrintWriter(this.port.getOutputStream());
			pw.write(payload);
			pw.close();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		this.lastPayload = payload;
		commLog.add("A: " + lastPayload);
	}
	
	public void parseResponse(byte[] response) {
		int parsedResponse = (int) response[0];
		if (parsedResponse == 0x01 /* ACK */) {
			System.out.println("Received ACK");
			commLog.add("B: ACK");
		} else if (parsedResponse == 0x0f /* NAK */) {
			System.out.println("Received NAK");
			commLog.add("B: NAK");
			String command = lastPayload.substring(0, lastPayload.indexOf(':'));
			int value = Integer.parseInt(lastPayload.substring(lastPayload.indexOf(':') + 1, lastPayload.length() - 1));
			write(new SerialCommand(command, value));
		} else {
			System.out.println("Got bullshit. Terminating...");
			destroy();
		}
	}
}
