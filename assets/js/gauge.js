

var Gauge = function (argmap) {
    if('name' in argmap) {
        this.name = argmap.name;
    }
    if('id' in argmap) {
        this.id = argmap.id;
    } else {
        this.id = this.name + 'Panel';
    }
    if('title' in argmap) {
        this.title = argmap.title;
    }
    if('title' in argmap) {
        this.description = argmap.description;
    }
    if('width' in argmap) {
        this.width = argmap.width;
    } else {
        this.width = '100%';
    }
    if('height' in argmap) {
        this.height = argmap.height;
    } else {
        this.height = '100%';
    }
    if('x' in argmap) {
        this.x = argmap.x;
    } else {
        x = '10px';
    }
    if('y' in argmap) {
        this.y = argmap.y;
    } else {
        y = '10px';
    }
    if('backgroundImg' in argmap) {
        this.backgroundImg = argmap.backgroundImg;
    }
    if('foregroundImg' in argmap) {
        this.foregroundImg = argmap.foregroundImg;
    }
    if('foregroundImgClips' in argmap) {
        this.foregroundImgClips = argmap.foregroundImgClips;
    } else {
        this.foregroundImgClips = false;
    }
    if('needleImg1' in argmap) {
        this.needleImg1 = argmap.needleImg1;
    }
    if('needleImg2' in argmap) {
        this.needleImg2 = argmap.needleImg2;
    }
    if('needleImg3' in argmap) {
        this.needleImg3 = argmap.needleImg3;
    }
    if('icon' in argmap) {
        this.icon = argmap.icon;
    }
    this.enabled = false;
}
    
Gauge.prototype = {
    setBackgroundImage: function(i, b) {
        b = (typeof b === "undefined") ? false : b;
        this.panelBackgroundImg = i;
        this.backgroundImgClips = b;
    },
    setForegroundImage: function(i) {
        this.panelForegroundImg = i;
    },
    setNeedle1Image: function(i) {
        this.panelNeedleImg1 = i;
    },
    setNeedle2Image: function(i) {
        this.panelNeedleImg2 = i;
    },
    setNeedle3Image: function(i) {
        this.panelNeedleImg3 = i;
    },
};
