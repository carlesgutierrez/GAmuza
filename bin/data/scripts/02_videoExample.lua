






-- loading video example

myVideo = of.videoPlayer()

function setup()
    myVideo:loadMovie("videos/video.mov")
    myVideo:play()
end

function update()
    myVideo:update()
end

function draw()
    ga.background(0.0,1.0)
    of.setColor(255,255,255)
    myVideo:draw(0,0,OUTPUT_W,OUTPUT_H)
end

function mouseMoved()

end

function mouseDragged()

end

function mousePressed()
    myVideo:setPaused(true)
end

function mouseReleased()
    myVideo:setPaused(false)
end















