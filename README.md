# This Demo Helps You To Avoid The Bothering Of Eulers Rotation Order From Yaw, Pitch And Roll

If you need to rotate your model along all 3 axes of Yaw, Pitch and Roll, the rotation order matters.
Rotate Pitch -> Yaw -> Roll and Rotate Yaw -> Pitch -> Roll will get you different matrices.

To avoid Euler Gimbal Lock, we would want to save Yaw, Pitch and Roll angles for later reference instead of saving rotated matrix.
Every rendering loop, we would calculate the matrix from the 3 rotation angles.

But when you about to apply new rotation to matrix, say, rotate the model along Yaw axis, you need to rotate Yaw angles instead of other 2 axes as the first step ahead of applying existing rotation angles.

The procedure would be(take Yaw as an example):
1. Rotate new Yaw angles to mat1.
2. Set former Yaw, Pitch and Roll angles to mat2 in a whole, using 'setEulerToMatrix()' in the demo.
3. Get mat3 = mat1 * mat2;
4. Extract Yaw, Pitch and Roll angles from mat3, using 'getEulerFromMatrix()' in the demo.
5. Save Yaw, Pitch and Roll angles for later reference.

When you need the model matrix for rendering:
1. Use 'setEulerToMatrix()' to do it, can avoid applying order of the 3 angles.
