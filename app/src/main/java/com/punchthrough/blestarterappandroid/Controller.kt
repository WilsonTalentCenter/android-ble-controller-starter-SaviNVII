package com.punchthrough.blestarterappandroid

import android.annotation.SuppressLint
import android.bluetooth.BluetoothDevice
import android.os.Bundle
import android.view.MotionEvent
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.punchthrough.blestarterappandroid.R
import com.punchthrough.blestarterappandroid.ble.ConnectionManager
import timber.log.Timber
import java.util.UUID

class Controller : AppCompatActivity() {

    // 1. Define the device variable at the top
    private lateinit var device: BluetoothDevice

    @SuppressLint("ClickableViewAccessibility")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_controller)

        // 2. Get the device passed from the previous activity
        device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE)
            ?: error("No BluetoothDevice found in intent")

        // 3. Link your UI Buttons (ensure these IDs match your XML)
       // val btnLedOn = findViewById<Button>(R.id.button_led_on)
        val btnMoveUp = findViewById<Button>(R.id.buttonMoveUp)
    }

    private fun sendToArduino(command: String) {
        // 1. Get the list of services from the ConnectionManager
        val services = ConnectionManager.servicesOnDevice(device)

        // 2. Find the specific HM-10 Serial Service
        val service = services?.find { it.uuid == UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb") }

        // 3. Find the specific HM-10 Serial Characteristic
        val characteristic = service?.getCharacteristic(UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb"))

        characteristic?.let { char ->
            val bytes = command.toByteArray(Charsets.UTF_8)
            // 4. Send it! The Manager handles the queueing.
            ConnectionManager.writeCharacteristic(device, char, bytes)
        } ?: run {
            Timber.e("HM-10 Serial characteristic not found!")
        }
    }
}
