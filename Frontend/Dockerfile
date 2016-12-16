FROM node:6.9.1

WORKDIR frontend

COPY package.json /frontend/
RUN npm install -g typescript webpack
RUN npm install
RUN npm link typescript

COPY tsconfig.json webpack.config.js /frontend/
ENTRYPOINT ["npm", "start"]