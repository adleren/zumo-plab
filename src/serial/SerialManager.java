/**
 * @author Adrian Leren
 *
 * Manager class that handles serial communication with a serial port.
 */
package serial;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

import com.fazecast.jSerialComm.SerialPort;

public class SerialManager {
	
	/* Private fields */
	private SerialPort port;
	private String lastPayload;
	private int retransmits;
	private List<String> commLog;
	
	/**
	 * Open serial port based on its index in the array collected from SerialPort.getCommPorts().
	 * 
	 * @param index of port in array collected from SerialPort.getCommPorts()
	 * @return 'true' if port was opened successfully
	 */
	public boolean openPort(int index) {
		this.port = SerialPort.getCommPorts()[index];
		port.setBaudRate(9600);
		
		this.retransmits = 0;
		
		commLog = new ArrayList<>();
		
		if (port.openPort()) {
			System.out.println("Successfully opened port at " + port.getDescriptivePortName());
			write(new SerialCommand("HI", 0));
			return true;
		} 
		System.err.println("Failed to open port '" + port.getDescriptivePortName() + "'");
		return false;
	}
	
	
	/**
	 * Deconstructor. Removes listeners and closes opened port if any.
	 */
	public void destroy() {
		port.removeDataListener();
		port.closePort();
		retransmits = 0;
		clearCommLog();
		System.out.println("SerialManager was destroyed...");
	}
	
	/**
	 * Gets the communication log.
	 * 
	 * @return Communication log
	 */
	public List<String> getCommLog() {
		return this.commLog;
	}
	
	
	/**
	 * Clears the communication log
	 */
	public void clearCommLog() {
		this.commLog = new ArrayList<>();
	}
	
	
	/**
	 * Checks if this manager is ready to read/write data to port.
	 * 
	 * @return 'true' if manager is ready.
	 */
	public boolean isReady() {
		return port != null && port.isOpen();
	}
	
	
	/**
	 * @return The selected port, or null.
	 */
	public SerialPort getPort() {
		return this.port;
	}
	
	
	/**
	 * Write data to port if possible.
	 * 
	 * @param sc The serial command to write.
	 */
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
	
	
	/**
	 * Upon receiving a response, parse this response and act appropriately.
	 * 
	 * @param response The received response.
	 */
	public void parseResponse(byte[] response) {
		int parsedResponse = (int) response[0];
		if (parsedResponse == 0x01 /* ACK */) {
			System.out.println("Received ACK");
			commLog.add("B: ACK");
		} else if (parsedResponse == 0x0f /* NAK */ && retransmits < 3) {
			System.out.println("Received NAK");
			commLog.add("B: NAK");
			
			try {
				Thread.sleep(1000);
				write(new SerialCommand(lastPayload));
				retransmits++;
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		} else {
			System.out.println("Got bullshit. Terminating...");
			destroy();
		}
	}
}
