
-- audio input example

inputVol = 0.0
inputPitch = 0.0

-----------------------
function setup()
    of.setCircleResolution(50)
end
-----------------------
function update()
    inputVol = ga.getVolume(0)  -- get volume from channel 0
    inputPitch = ga.getPitch(0) -- get pitch from channel 0
end
-----------------------
function draw()
    
    ga.background(0.0,0.03)

    of.setColor(255,2550*inputPitch,0)
    of.fill()
    of.circle(OUTPUT_W/2 + (inputPitch*1000),OUTPUT_H/2,inputVol*2000)

    of.setColor(255,255,255)
    of.noFill()

    -- draw fft bins
    for i=0,FFT_BANDS do
        of.rect(i*(OUTPUT_W/FFT_BANDS),OUTPUT_H,OUTPUT_W/FFT_BANDS,-OUTPUT_H*ga.getBin(0,i))
    end
end







