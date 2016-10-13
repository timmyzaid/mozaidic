BUILD_TYPE = Release

build-image-build-image:
	cd Dockerfiles/Image\ Service/Buildfile \
		&& docker build -t image_build:latest .

build-image-service: build-image-build-image
	docker run -i -t --rm -v ${CURDIR}/Image\ Service/:/Image\ Service -v ${CURDIR}/Dockerfiles/Image\ Service/Runfile/:/dependencies -e "BUILD_TYPE=$(BUILD_TYPE)" --entrypoint=/build.sh image_build

build-image-service-image: build-image-service
	cd Dockerfiles/Image\ Service/Runfile \
		&& docker build -t image_service:latest .

clean:
	rm -rf Image\ Service/build
	rm -rf Dockerfiles/Image\ Service/Runfile/build
