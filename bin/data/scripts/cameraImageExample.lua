



-- camera Image Example

test = of.texture()

function setup()
    test:allocate(CAPTURE_W,CAPTURE_H,GL_RGB)
end

function update()
    test = ga.cameraTexture(0)
end

function draw()
    ga.background(0.0,0.2)
    of.setColor(255)
    test:draw(of.mouseX(),of.mouseY())
end































