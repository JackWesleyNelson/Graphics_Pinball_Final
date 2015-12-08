class CircularBoardObject {
public:
	CircularBoardObject();
	CircularBoardObject(int tx, int ty, int tz, int tr);
	CircularBoardObject(float tx, float ty, float tz, float tr);

	double getX();
	void setX(double xt);
	double getY();
	void setY(double yt);
	double getZ();
	void setZ(double zt);
	double getRadius();
	void setRadius(double rt);
	
private:
	double x, y, z;
	double radius;
};