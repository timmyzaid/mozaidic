BUILD_TYPE = Release

image-build-image:
	cd Dockerfiles/Image\ Service/Buildfile \
		&& docker build -t image_build:latest .

build-image-service: image-build-image
	docker run -i -t --rm -v ${CURDIR}/Image\ Service/:/Image\ Service -v ${CURDIR}/Dockerfiles/Image\ Service/Runfile/:/dependencies -e "BUILD_TYPE=$(BUILD_TYPE)" --entrypoint=/build.sh image_build

image-service-image: build-image-service
	cd Dockerfiles/Image\ Service/Runfile \
		&& docker build -t image_service:latest .

upload-service-image:
	cp Dockerfiles/Upload\ Service/Dockerfile Upload\ Service/
	cd Upload\ Service \
		&& docker build -t upload_service:latest .
	rm Upload\ Service/Dockerfile

nginx-image:
	cp Dockerfiles/nginx/Dockerfile nginx/
	cd nginx \
		&& docker build -t mozaidic_nginx:latest .
	rm nginx/Dockerfile

clean:
	rm -rf Image\ Service/build
	rm -rf Dockerfiles/Image\ Service/Runfile/build
