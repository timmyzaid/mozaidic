import * as React from 'react';



function handleUpload(event: React.FormEvent<HTMLInputElement>) {
    console.log(event);

    let request = new XMLHttpRequest();
    request.onerror = (ev: ErrorEvent) => {
        console.log(ev);
    };
    request.onload = (ev: Event) => {
        console.log(ev);
    };
    request.open('POST', '/upload_image');
    request.send();
}

export function UploadButton({containerClass, labelClass, inputClass, buttonText}: UploadButtonProps) {
    return (
        <div className={containerClass}>
            <label htmlFor="file-upload" className={labelClass}>{buttonText}</label>
            <input id="file-upload" className={inputClass} type="file" onChange={handleUpload} />
        </div>
    );
}