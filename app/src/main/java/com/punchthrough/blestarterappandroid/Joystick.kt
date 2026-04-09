package com.punchthrough.blestarterappandroid

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import kotlin.math.atan2
import kotlin.math.min
import kotlin.math.sqrt

class JoystickView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : View(context, attrs, defStyleAttr) {

    /** Fired on every touch move and on release (angle=0, strength=0). */
    var onMoveListener: ((angle: Int, strength: Int) -> Unit)? = null

    private val basePaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = Color.argb(51, 255, 255, 255)
        style = Paint.Style.FILL
    }
    private val borderPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = Color.WHITE
        style = Paint.Style.STROKE
        strokeWidth = 4f
    }
    private val thumbPaint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        color = 0xFF00B0FF.toInt()
        style = Paint.Style.FILL
    }
    private var centerX = 0f
    private var centerY = 0f
    private var baseRadius = 0f
    private var thumbRadius = 0f

    // Floating origin — rests at view center, snaps to first touch point on ACTION_DOWN
    private var originX = 0f
    private var originY = 0f
    private var thumbX = 0f
    private var thumbY = 0f

    companion object {
        private const val BASE_RATIO = 0.40f
        private const val THUMB_RATIO = 0.35f
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        centerX = w / 2f
        centerY = h / 2f
        baseRadius = min(w, h) / 2f * BASE_RATIO
        thumbRadius = baseRadius * THUMB_RATIO
        originX = centerX
        originY = centerY
        thumbX = centerX
        thumbY = centerY
    }

    override fun onDraw(canvas: Canvas) {
        canvas.drawCircle(originX, originY, baseRadius, basePaint)
        canvas.drawCircle(originX, originY, baseRadius, borderPaint)
        canvas.drawCircle(thumbX, thumbY, thumbRadius, thumbPaint)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                // Clamp origin so the base circle never clips outside the view
                originX = event.x.coerceIn(baseRadius, width - baseRadius)
                originY = event.y.coerceIn(baseRadius, height - baseRadius)
                thumbX = originX
                thumbY = originY
                updateThumb(event.x, event.y)
            }
            MotionEvent.ACTION_MOVE -> updateThumb(event.x, event.y)
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> release()
        }
        return true
    }

    private fun updateThumb(touchX: Float, touchY: Float) {
        val dx = touchX - originX
        val dy = touchY - originY
        val distance = sqrt(dx * dx + dy * dy)
        val clamped = min(distance, baseRadius)
        val scale = if (distance > 0f) clamped / distance else 0f

        thumbX = originX + dx * scale
        thumbY = originY + dy * scale

        val angleRad = atan2(-dy.toDouble(), dx.toDouble())
        val angleDeg = ((Math.toDegrees(angleRad) + 360) % 360).toInt()
        val strength = (clamped / baseRadius * 100).toInt()
        onMoveListener?.invoke(angleDeg, strength)
        invalidate()
    }

    private fun release() {
        originX = centerX
        originY = centerY
        thumbX = centerX
        thumbY = centerY
        onMoveListener?.invoke(0, 0)
        invalidate()
    }
}
 