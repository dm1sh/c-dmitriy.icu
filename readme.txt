Webserver in c. !Experimental!

Deployment:

Docker and Heroku deployment are supported. To do Heroku deployment use https://github.com/heroku/heroku-buildpack-c buildpack. For Docker just build container image and run it exposing port:

docker build -t c-dmitriy.icu .
docker run -dp 5000:5000 c-dmitriy.icu