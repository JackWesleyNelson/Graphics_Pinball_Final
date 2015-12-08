class RectangularBoardObject {
public:
	// Constructors
	RectangularBoardObject();
	RectangularBoardObject(int tx, int ty, int tz, int tdx, int tdz);
	RectangularBoardObject(float tx, float ty, float tz, float tdx, float tdz);

	// Getters and Setters
	double getX();
	void setX(double xt);
	double getY();
	void setY(double yt);
	double getZ();
	void setZ(double zt);
	double getDeltaX();
	void setDeltaX(double dxt);
	double getDeltaZ();
	void setDeltaZ(double dzt);
	
private:
	double x, y, z;
	double deltaX, deltaZ;
};