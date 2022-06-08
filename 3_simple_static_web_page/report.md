<!-- TOC -->

- [1. Introduction and personal ideas](#1-introduction-and-personal-ideas)
- [2. Explaination of the HTML file](#2-explaination-of-the-html-file)
- [3. Explaination of the CSS file](#3-explaination-of-the-css-file)
- [4. Pictures of the website](#4-pictures-of-the-website)

<!-- /TOC -->
# 1. Introduction and personal ideas
I am always passionate about having my very own website, so this task is of great interest to me, as other tasks in this lecture. I highly value the opportunity of this assignment, and I crammed a lot of HTML and CSS knowledge to write this website. I want to say that so far, this is the most exciting class I have taken since I came to HSKA. Note that I have been working on this website, the version I submitted as a report is a few days old, but I think it is good enough to cope with this assignment.

HTML and CSS, together with Javascript, compose most modern websites. HTML structures the website, CSS decorates the website, while Javascript functions the website.

# 2. Explaination of the HTML file
Since the functionality of HTML is to structure the website, in this section, I will introduce the structure of my website with the HTML code and pictures. The principle of constructing a website structure using HTML is to create many boxes. These boxes are like containers, separating the inside from the outside. You can define CSS styles inside the container. The styles defined inside will not affect the outside, but the inside sometimes inherits the CSS styles from the outside. 

![demo](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/demo.png)

The structure of my website is shown in the figure. The site is mainly composed of a Header and Body. The Header container contains just one navigation bar, which is actually a bullet list. Through CSS programming, the bulleted list can be displayed like this. There are three main sub-containers inside the Body, bannerRegion, aboutME and selfIntroduction, each of which contains its own sub-container. In this way, the HTML structure of the website is built by stacking the containers.

![20220412220557](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220412220557.png)
Here in the picture is the header part.

![20220412220936](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220412220936.png)

And also the picture for the body part.

# 3. Explaination of the CSS file
The CSS is used to decorate the HTLM file. It can only beautify the website but can not change the structure anymore. In this section, only several important techniques I used will be demonstrated.

![20220412221214](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220412221214.png)

To construct a navigation bar from a bullet list, you must first remove the "bullets" in the bullet list, which is done by setting the value of "text-decoration" to be none. And since the bullet list is vertical, you have to make it horizontal; this is realized by making the block element flow to the left. Once the block can flow, it will move to the line above it and never take up spaces below. Thus, a bullet list can be converted into a horizontal bar.

![20220412221649](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220412221649.png)

The "clear" item is to remove the flow effect. When using one container to accommodate multiple containers in one line, the parent container might collapse, which resulting the parent element cannot visually wrap around the child element.

![20220412222203](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220412222203.png)

For this Headbar code, by setting the position to be "fixed," the navigation bar in the header will hold its place at the top of the browser and never change its location.

# 4. Pictures of the website
![website](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/website.png)