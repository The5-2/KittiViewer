#version 330

precision highp float;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

uniform mat4 uProjectionVideo;


/*All matrices*/
uniform vec3 scale;
uniform vec3 translate;
uniform float angle;

uniform int uRotMode;
uniform int uTransMode;
uniform int uScaleMode;

uniform mat4 P_rect_02;
uniform mat4 R_rect_02;
uniform mat4 R_rect_00;
uniform mat4 transform_cam00_to_cam02;
uniform mat4 transform_velo_to_cam00;
uniform mat4 rect;

uniform vec4 uColor;

uniform float uWidth;
uniform float uHeight;

layout(location = 0) in  vec3 vPosition; 

out float vClip;
out vec4 oColor;

void main(){
	oColor = uColor;

	vec4 vPosW;
	vec4 posInVideoSpace;

	vPosW = uModelMatrix * vec4(vPosition, 1.0);

	// posInVideoSpace = uProjectionVideo * vPosW;
	posInVideoSpace = P_rect_02 * R_rect_00 * transform_velo_to_cam00 * vPosW;
	// posInVideoSpace = P_rect_02 * R_rect_02 * transform_cam00_to_cam02 * transform_velo_to_cam00 * vPosW;
	// posInVideoSpace = rect * vPosW;

	vClip = step(posInVideoSpace.z,1.0);

	posInVideoSpace.xyz /= posInVideoSpace.z;
	posInVideoSpace.z = 0.0;
	posInVideoSpace.w = 1.0;

	posInVideoSpace.x = posInVideoSpace.x / 1242.0;
	posInVideoSpace.y = posInVideoSpace.y / 375.0;	
	posInVideoSpace.y = 1.0 - posInVideoSpace.y;
	posInVideoSpace.xy = 2.0 * posInVideoSpace.xy - vec2(1.0);
	gl_Position = posInVideoSpace;
}

// Tr_velo_to_cam = np.zeros((3, 4))
// Tr_velo_to_cam[0:3, 0:3] = np.reshape(velo2cam["R"], [3, 3])
// Tr_velo_to_cam[:, 3] = velo2cam["T"]
// data["Tr_velo_to_cam"] = np.reshape(Tr_velo_to_cam, [12])
// data["R0_rect"] = cam2cam["R_rect_00"]
// data["P2"] = cam2cam["P_rect_02"]

// https://github.com/kuixu/kitti_object_vis/blob/85412633478a60e9a972fb2e012d65e7a6f1a140/kitti_util.py#L190
// def compute_box_3d(obj, P):
//     """ Takes an object and a projection matrix (P) and projects the 3d
//         bounding box into the image plane.
//         Returns:
//             corners_2d: (8,2) array in left image coord.
//             corners_3d: (8,3) array in in rect camera coord.
//     """
//     # compute rotational matrix around yaw axis
//     R = roty(obj.ry)

//     # 3d bounding box dimensions
//     l = obj.l
//     w = obj.w
//     h = obj.h

//     # 3d bounding box corners
//     x_corners = [l / 2, l / 2, -l / 2, -l / 2, l / 2, l / 2, -l / 2, -l / 2]
//     y_corners = [0, 0, 0, 0, -h, -h, -h, -h]
//     z_corners = [w / 2, -w / 2, -w / 2, w / 2, w / 2, -w / 2, -w / 2, w / 2]

//     # rotate and translate 3d bounding box
//     corners_3d = np.dot(R, np.vstack([x_corners, y_corners, z_corners]))
//     # print corners_3d.shape
//     corners_3d[0, :] = corners_3d[0, :] + obj.t[0]
//     corners_3d[1, :] = corners_3d[1, :] + obj.t[1]
//     corners_3d[2, :] = corners_3d[2, :] + obj.t[2]
//     # print 'cornsers_3d: ', corners_3d
//     # only draw 3d bounding box for objs in front of the camera
//     if np.any(corners_3d[2, :] < 0.1):
//         corners_2d = None
//         return corners_2d, np.transpose(corners_3d)

//     # project the 3d bounding box into the image plane
//     corners_2d = project_to_image(np.transpose(corners_3d), P)
//     # print 'corners_2d: ', corners_2d
//     return corners_2d, np.transpose(corners_3d)