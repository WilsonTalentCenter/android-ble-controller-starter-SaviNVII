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
import kotlin.math.cos
import kotlin.math.sin

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
        val joystick = findViewById<JoystickView>(R.id.joystickView)
        joystick.onMoveListener = { angle, strength ->
            sendJoystickPosition(angle, strength)
        }
        val btnBodyClockwise = findViewById<Button>(R.id.buttonRotateBodyClockwise)
        btnBodyClockwise.setOnTouchListener { view, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    sendToArduino("B,CW\n")
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    sendToArduino("B,0\n")
                    true
                }
                else -> false
            }
        }
        val btnBodyCounterClockwise = findViewById<Button>(R.id.buttonRotateBodyCounterClockwise)
        btnBodyCounterClockwise.setOnTouchListener { view, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    sendToArduino("B,CCW\n")
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    sendToArduino("B,0\n")
                    true
                }
                else -> false
            }
        }
        val btnShieldClockwise = findViewById<Button>(R.id.buttonRotateShieldClockwise)
        btnShieldClockwise.setOnTouchListener { view, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    sendToArduino("S,CW\n")
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    sendToArduino("S,0\n")
                    true
                }
                else -> false
            }
        }
        val btnShieldCounterClockwise = findViewById<Button>(R.id.buttonRotateShieldCounterClockwise)
        btnShieldCounterClockwise.setOnTouchListener { view, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    sendToArduino("S,CCW\n")
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    sendToArduino("S,0\n")
                    true
                }
                else -> false
            }
        }
    }

    private fun sendJoystickPosition(angle: Int, strength: Int) {
        val rad = Math.toRadians(angle.toDouble())
        val nx = cos(rad) * strength / 100.0
        val ny = -sin(rad) * strength / 100.0
        var bx = ((nx + 1.0) * 127.5).toInt().coerceIn(0, 255)
        var by = ((ny + 1.0) * 127.5).toInt().coerceIn(0, 255)
        bx -= 127
        by -= 127
        by *= -1
        sendToArduino("J,$bx,$by\n")
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
