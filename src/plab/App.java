/**
 * @author Adrian Leren
 *
 * Main entry-point for JavaFX UI 
 */
package plab;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

public class App extends Application {

	@Override
	public void start(Stage stage) throws Exception {
		Parent root = FXMLLoader.load(getClass().getResource("UI.fxml"));
        Scene scene = new Scene(root, 600, 400);
        stage.setTitle("Zumo UI");
        stage.setScene(scene);
        
		stage.setOnCloseRequest(new EventHandler<WindowEvent>() {
			@Override
			public void handle(WindowEvent event) {
				Platform.exit();
				System.exit(0);
			}
		});
        
        stage.show();
	}

	public static void main(String[] args) {
		launch(args);
	}

}
