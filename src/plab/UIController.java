package plab;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import serial.SerialCommand;
import serial.SerialManager;

public class UIController {
	
	SerialManager serialManager;

	@FXML Button connectBtn;
	@FXML Button sendBtn;
	
	@FXML Button clearBtn;
	
	@FXML Button presetBtn1;
	@FXML Button presetBtn2;
	@FXML Button presetBtn3;
	@FXML Button presetBtn4;
	
	@FXML Slider fsSlider;
	@FXML Slider rsSlider;
	@FXML Slider tsSlider;
	@FXML Slider rdSlider;
	@FXML Slider tdSlider;
	
	@FXML ComboBox<String> portsBox;
	
	@FXML TextArea logArea;
	@FXML TextField commandField;
	@FXML Text activityText;
	
	
	public void initialize() {
		serialManager = new SerialManager();
		
		List<String> portnames = new ArrayList<>();
		for (SerialPort port : SerialPort.getCommPorts()) {
			portnames.add(port.getDescriptivePortName());
		}
		portsBox.setItems(FXCollections.observableArrayList(portnames));
		sendBtn.setDisable(true);
		
		fsSlider.valueChangingProperty().addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observableValue, Boolean wasChanging, Boolean changing) {
				if (!changing) {
					int value = (int) fsSlider.getValue();
					commandField.setText("FS:" + value + "#");
				}
			}
		});
		rsSlider.valueChangingProperty().addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observableValue, Boolean wasChanging, Boolean changing) {
				if (!changing) {
					int value = (int) rsSlider.getValue();
					commandField.setText("RS:" + value + "#");
				}
			}
		});
		tsSlider.valueChangingProperty().addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observableValue, Boolean wasChanging, Boolean changing) {
				if (!changing) {
					int value = (int) tsSlider.getValue();
					commandField.setText("TS:" + value + "#");
				}
			}
		});
		rdSlider.valueChangingProperty().addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observableValue, Boolean wasChanging, Boolean changing) {
				if (!changing) {
					int value = (int) rdSlider.getValue();
					commandField.setText("RD:" + value + "#");
				}
			}
		});
		tdSlider.valueChangingProperty().addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observableValue, Boolean wasChanging, Boolean changing) {
				if (!changing) {
					int value = (int) tdSlider.getValue();
					commandField.setText("TD:" + value + "#");
				}
			}
		});
	}
	
	public void updateLog() {
		String log = "";
		for (String line : serialManager.getCommLog()) {
			log += line + "\n";
		}
		logArea.setText(log);
	}
	
	@FXML
	public void onConnectPressed() {
		if (!serialManager.isReady()) {
			serialManager.openPort(portsBox.getSelectionModel().getSelectedIndex());
			connectBtn.setText("Disconnect");
			sendBtn.setDisable(false);
			activityText.setText("Active");
			activityText.setFill(Color.GREEN);
			
			serialManager.getPort().addDataListener(new SerialPortDataListener() {
				@Override
				public int getListeningEvents() { return SerialPort.LISTENING_EVENT_DATA_AVAILABLE; }
				
				@Override
					public void serialEvent(SerialPortEvent event) {
					if (event.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
						return;
					byte[] newData = new byte[serialManager.getPort().bytesAvailable()];
					int numRead = serialManager.getPort().readBytes(newData, newData.length);
					System.out.println("Read " + numRead + " bytes, " + Arrays.toString(newData));
					serialManager.parseResponse(newData);
					updateLog();
				}
			});
		} else {
			serialManager.destroy();
			connectBtn.setText("Connect");
			sendBtn.setDisable(true);
			activityText.setText("Inactive");
			activityText.setFill(Color.RED);
		}
	}
	
	@FXML
	public void onSendPressed() {
		if (serialManager.isReady())
			serialManager.write(new SerialCommand(commandField.getText()));
	}
	
	@FXML
	public void onClearPressed() {
		serialManager.clearCommLog();
		logArea.setText("");
		updateLog();
	}
	
	@FXML
	public void onPresetBtn1Pressed() {
		
	}
	
	@FXML
	public void onPresetBtn2Pressed() {
		
	}
	
	@FXML
	public void onPresetBtn3Pressed() {
		
	}
	
	@FXML
	public void onPresetBtn4Pressed() {
		
	}

}
