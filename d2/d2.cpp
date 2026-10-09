#include "d2.h"

d2l::d2l(void) {}

d2l::~d2l(void) {}

void drawSkewed(p3d p, p3d sz, int MODE, gm44d m)
{
    glBegin(GL_QUADS);
    cv::Point3d q = p;
    q.x = -p.x;
    cv::Point3d r = sz / 2, a = q + r, s = q - r;
    MODE = 1;

    glNormal3d(1, 0, 0);
    glTexCoord2d(1, 1);
    glv3d(m * p3d(-s.x, s.y, a.z));
    glTexCoord2d(0, 1);
    glv3d(m * p3d(-s.x, s.y, s.z));
    glTexCoord2d(0, 0);
    glv3d(m * p3d(-s.x, a.y, s.z));
    glTexCoord2d(1, 0);
    glv3d(m * p3d(-s.x, a.y, a.z)); //左

    glNormal3d(-1, 0, 0);
    glTexCoord2d(1, 1);
    glv3d(m * p3d(-a.x, s.y, s.z));
    glTexCoord2d(0, 1);
    glv3d(m * p3d(-a.x, s.y, a.z));
    glTexCoord2d(0, 0);
    glv3d(m * p3d(-a.x, a.y, a.z));
    glTexCoord2d(1, 0);
    glv3d(m * p3d(-a.x, a.y, s.z)); //右

    glNormal3d(0, -1, 0);
    glTexCoord2d(1, 1);
    glv3d(m * p3d(-a.x, s.y, s.z));
    glTexCoord2d(0, 1);
    glv3d(m * p3d(-s.x, s.y, s.z));
    glTexCoord2d(0, 0);
    glv3d(m * p3d(-s.x, s.y, a.z));
    glTexCoord2d(1, 0);
    glv3d(m * p3d(-a.x, s.y, a.z)); //下

    glNormal3d(0, 1, 0);
    glTexCoord2d(0, 0);
    glv3d(m * p3d(-a.x, a.y, a.z));
    glTexCoord2d(1, 0);
    glv3d(m * p3d(-s.x, a.y, a.z));
    glTexCoord2d(1, 1);
    glv3d(m * p3d(-s.x, a.y, s.z));
    glTexCoord2d(0, 1);
    glv3d(m * p3d(-a.x, a.y, s.z)); //上

    glNormal3d(0, 0, -1);
    glTexCoord2d(0, 0);
    glv3d(m * p3d(-a.x, a.y, s.z));
    glTexCoord2d(1, 0);
    glv3d(m * p3d(-s.x, a.y, s.z));
    glTexCoord2d(1, 1);
    glv3d(m * p3d(-s.x, s.y, s.z));
    glTexCoord2d(0, 1);
    glv3d(m * p3d(-a.x, s.y, s.z)); //后

    glNormal3d(0, 0, 1);
    glTexCoord2d(1, 1);
    glv3d(m * p3d(-a.x, s.y, a.z));
    glTexCoord2d(0, 1);
    glv3d(m * p3d(-s.x, s.y, a.z));
    glTexCoord2d(0, 0);
    glv3d(m * p3d(-s.x, a.y, a.z));
    glTexCoord2d(1, 0);
    glv3d(m * p3d(-a.x, a.y, a.z)); //前

    glEnd();
}

void glv3d(p3d e) { glVertex3d(-e.x, e.y, e.z); }

void drtg(p3d a, p3d b, p3d c, p3d d)
{
    glBegin(GL_QUADS);
    glTexCoord2d(0, 1);
    glv3d(a);
    glTexCoord2d(1, 1);
    glv3d(b);
    glTexCoord2d(1, 0);
    glv3d(d);
    glTexCoord2d(0, 0);
    glv3d(c);
    glEnd();
}

void drtg(std::deque<obj> t)
{
    for (ulong i = 0; i < t.size(); i++) { drtg(t[i]); }
}

void drtg(obj t) { drtg(t.e, t.sz, t.l3, t.m); }

void drtg(p3d p, p3d sz, GLuint tex, gm44d m)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    drawSkewed(p, sz, 0, m);
    glDisable(GL_TEXTURE_2D);
}

void drawSkewed(conr c, int MODE)
{
    glBegin(GL_QUADS);
    MODE = 1;

    glNormal3d(1, 0, 0);
    glTexCoord2d(1, 0);
    glv3d(c.pt[0][0][0][0]);
    glTexCoord2d(0, 0);
    glv3d(c.pt[0][0][1][0]);
    glTexCoord2d(0, 1);
    glv3d(c.pt[0][1][1][0]);
    glTexCoord2d(1, 1);
    glv3d(c.pt[0][1][0][0]); //左

    glNormal3d(-1, 0, 0);
    glTexCoord2d(1, 0);
    glv3d(c.pt[1][1][0][0]);
    glTexCoord2d(0, 0);
    glv3d(c.pt[1][1][1][0]);
    glTexCoord2d(0, 1);
    glv3d(c.pt[1][0][1][0]);
    glTexCoord2d(1, 1);
    glv3d(c.pt[1][0][0][0]); //右

    glNormal3d(0, -1, 0);
    glTexCoord2d(1, 0);
    glv3d(c.pt[1][0][0][0]);
    glTexCoord2d(0, 0);
    glv3d(c.pt[1][0][1][0]);
    glTexCoord2d(0, 1);
    glv3d(c.pt[0][0][1][0]);
    glTexCoord2d(1, 1);
    glv3d(c.pt[0][0][0][0]); //下

    glNormal3d(0, 1, 0);
    glTexCoord2d(0, 1);
    glv3d(c.pt[0][1][0][0]);
    glTexCoord2d(1, 1);
    glv3d(c.pt[0][1][1][0]);
    glTexCoord2d(1, 0);
    glv3d(c.pt[1][1][1][0]);
    glTexCoord2d(0, 0);
    glv3d(c.pt[1][1][0][0]); //上

    glNormal3d(0, 0, -1);
    glTexCoord2d(0, 1);
    glv3d(c.pt[0][0][0][0]);
    glTexCoord2d(1, 1);
    glv3d(c.pt[0][1][0][0]);
    glTexCoord2d(1, 0);
    glv3d(c.pt[1][1][0][0]);
    glTexCoord2d(0, 0);
    glv3d(c.pt[1][0][0][0]); //后

    glNormal3d(0, 0, 1);
    glTexCoord2d(1, 0);
    glv3d(c.pt[1][0][1][0]);
    glTexCoord2d(0, 0);
    glv3d(c.pt[1][1][1][0]);
    glTexCoord2d(0, 1);
    glv3d(c.pt[0][1][1][0]);
    glTexCoord2d(1, 1);
    glv3d(c.pt[0][0][1][0]); //前

    glEnd();
}

void drawBall(p3d p, p3d sz, scalar cl, int MODE)
{
    glTranslated(-p.x, p.y, p.z);
    if (MODE == 1)
    {
#ifndef FYAIRO_1_0_0_ARM
        glColor3d(cl[2] / 255, cl[1] / 255, cl[0] / 255);
        glutSolidSphere(sz.x, 40, 40);
#endif
    }
    else if (MODE == 2)
    {
        glColor3d(cl[2] / 255, cl[1] / 255, cl[0] / 255);
#ifndef FYAIRO_1_0_0_ARM
        glutWireSphere(sz.x, 40, 40);
#endif
    }
    glTranslated(p.x, -p.y, -p.z);
}

void drawHalfBall(p3d p, p3d sz, int MODE)
{
    glTranslated(-p.x, p.y, p.z);
    GLdouble eqn[4] = {0.0, 1.0, 0.0, 0.0};
    glClipPlane(GL_CLIP_PLANE0, eqn);
    glEnable(GL_CLIP_PLANE0);
    if (MODE == 1)
    {
#ifndef FYAIRO_1_0_0_ARM
        glutSolidSphere(sz.x, 40, 40);
#endif
    }
    else if (MODE == 2)
    {
#ifndef FYAIRO_1_0_0_ARM
        glutWireSphere(sz.x, 40, 40);
#endif
    }
    glDisable(GL_CLIP_PLANE0);
}

void drawBall(ddot a) { drawBall(a.e, p3d(a.sz), a.cl, 1); }

void drtg1(obj &t0, int MODE)
{
    glPushMatrix();
    if (t0.sp == 0) drawSkewed(t0.c, MODE);
    if (t0.sp == 1) drawBall(t0.e, t0.sz, t0.cl, MODE);
    if (t0.sp == 2) drawHalfBall(t0.e, t0.sz, MODE);
    glPopMatrix();
}

void dotg(ddot &d) { dotg(d.e, d.cl); }

void dotg(double ex, double ey, double ez, double cr, double cg, double cb)
{
    glColor3d(cr / 255.0, cg / 255.0, cb / 255.0);
    glVertex3d(-ex, ey, ez);
}

void dotg(p3d &e, scalar &cl) { dotg(e.x, e.y, e.z, cl.val[2], cl.val[1], cl.val[0]); }

void dline(axis d)
{
    glLineWidth(d.linewidth);
    glBegin(GL_LINES);
    glColor3d(d.ca.val[2] / 255, d.ca.val[1] / 255, d.ca.val[0] / 255);
    glVertex3d(-d.a.x, d.a.y, d.a.z);
    glColor3d(d.cb.val[2] / 255, d.cb.val[1] / 255, d.cb.val[0] / 255);
    glVertex3d(-d.b.x, d.b.y, d.b.z);
    glEnd();
}

int d2l::calib3d(cv::Mat &matl, cv::Mat &matr, string msg[], cv::Mat &fml, cv::Mat &fmr)
{
    using namespace std;
    using namespace cv;
    imgsz.width = matl.cols;
    imgsz.height = matl.rows;

    int rtl = findChessboardCornersSB(matl, ptsz, corner_points_bufl, CALIB_CB_ACCURACY),
        rtr = findChessboardCornersSB(matr, ptsz, corner_points_bufr, CALIB_CB_ACCURACY);
    if (!rtl || !rtr)
    {
        msg[0] = "没看见棋盘格";
        return 0;
    }
    ulong bfsz = ulong(ptsz.width * ptsz.height);
    float d = 11.0f;

    if (rtl && image_numl < _npic_)
    {
        imageInputl[image_numl] = matl.clone();
        cv::Mat grayl;
        cv::cvtColor(imageInputl[image_numl], grayl, cv::COLOR_BGR2GRAY);
        for (ulong i = 0; i < bfsz; i++)
        {
            if (corner_points_bufl[i].x < d || corner_points_bufl[i].x > imgsz.width - d ||
                corner_points_bufl[i].y < d || corner_points_bufl[i].y > imgsz.height - d)
            {
                return 0; // cvbug:   find4QuadCornerSubpix会报错;
            }
        }
        // cv::find4QuadCornerSubpix(grayl, corner_points_bufl, cv::Size(10,5));
        cornerpsl.push_back(corner_points_bufl);
        cv::drawChessboardCorners(grayl, ptsz, corner_points_bufl, true);
        matl = cvtcolor(grayl);
        fml = imageInputl[image_numl];
        msg[0] = "左 " + str(int(image_numl + 1)) + "/ " + str(_npic_);
        image_numl++;
    }

    if (rtr && image_numr < _npic_)
    {
        imageInputr[image_numr] = matr.clone();
        cv::Mat grayr;
        cv::cvtColor(imageInputr[image_numr], grayr, cv::COLOR_BGR2GRAY);
        for (ulong i = 0; i < bfsz; i++)
        {
            if (corner_points_bufr[i].x < d || corner_points_bufr[i].x > imgsz.width - d ||
                corner_points_bufr[i].y < d || corner_points_bufr[i].y > imgsz.height - d)
            {
                return 0; // cvbug:   find4QuadCornerSubpix会报错;
            }
        }
        // cv::find4QuadCornerSubpix(grayr, corner_points_bufr, cv::Size(10,5));
        cornerpsr.push_back(corner_points_bufr);
        cv::drawChessboardCorners(grayr, ptsz, corner_points_bufr, true);
        matr = cvtcolor(grayr);
        fmr = imageInputr[image_numr];
        msg[1] = "右 " + str(int(image_numr + 1)) + "/ " + str(_npic_);
        image_numr++;
    }

    if (image_numl < _npic_ || image_numr < _npic_) return 0;

    ulong totall = cornerpsl.size();
    ulong totalr = cornerpsr.size();
    msg[2] = "total=" + str(int(totall)) + "," + str(int(totalr));

    msg[0] = "角点提取完成";

    msg[1] = "开始标定………………";
    /**每幅图像的平移向量，t*/
    vector<cv::Mat> tvecsMatl, tvecsMatr;
    /**每幅图像的旋转向量（罗德里格旋转向量*/
    vector<cv::Mat> rvecsMatl, rvecsMatr;
    /**保存所有图片的角点的三维坐标*/
    vector<vector<cv::Point3f>> objectPoints;
    /**初始化每一张图片中标定板上角点的三维坐标*/
    int i, j, k;
    /**遍历每一张图片*/
    for (k = 0; k < _npic_; k++)
    {
        /**每一幅图片对应的角点数组*/
        vector<cv::Point3f> tempCornerPoints;
        /**遍历所有的角点*/
        for (i = 0; i < ptsz.height; i++)
        {
            for (j = 0; j < ptsz.width; j++)
            { /**一个角点的坐标*/
                float mmf = float(mm);
                tempCornerPoints.push_back(
                    cv::Point3f(j * 25 * mmf + -120 * mmf /*+-20*(2-k)*mmf*/, //-160mm,-140mm,-120mm
                                i * 25 * mmf + 20 * mmf /*+20*(6-l)*mmf*/,    // 20mm,40mm,...140mm
                                0                                             /*284*mmf*/
                                ));
            }
        }
        objectPoints.push_back(tempCornerPoints);
    }

    cv::calibrateCamera(objectPoints, cornerpsl, imgsz, cameraMatrix[0], distCoefficients[0], rvecsMatl, tvecsMatl, 0);

    usleep(20 * 1000);

    cv::calibrateCamera(objectPoints, cornerpsr, imgsz, cameraMatrix[1], distCoefficients[1], rvecsMatr, tvecsMatr, 0);

    msg[1] = "标定完成";
    /**保存标定结果的文件*/
    //    ofstream  ofout("/home/root/eye/dt/em/"+"0"+"caliberation_result.txt");
    /**开始保存标定结果*/
    msg[2] = "开始保存标定结果";
    cout << endl << "相机相关参数：" << endl;
    // ofout << "相机相关参数：" << endl;
    cout << "1.内外参数矩阵:" << endl;
    // ofout << "1.内外参数矩阵:" << endl;
    cout << "大小：" << cameraMatrix[0].size() << "," << cameraMatrix[1].size() << endl;
    // ofout << "大小：" << cameraMatrix[0].size()<<","<< cameraMatrix[1].size() << endl;
    cout << cameraMatrix[0] << "," << cameraMatrix[1] << endl;
    // ofout << cameraMatrix[0] <<","<< cameraMatrix[1] << endl;

    cout << "2.畸变系数：" << endl;
    // ofout << "2.畸变系数：" << endl;
    cout << "大小：" << distCoefficients[0].size() << "," << distCoefficients[1].size() << endl;
    // ofout << "大小：" << distCoefficients[0].size() <<","<< distCoefficients[1].size() << endl;
    cout << distCoefficients[0] << "," << distCoefficients[1] << endl;
    // ofout << distCoefficients[0] <<","<< distCoefficients[1] << endl;

    cout << endl << "图像相关参数：" << endl;
    // ofout << endl << "图像相关参数：" << endl;
    /**旋转矩阵*/
    cv::Mat rotation_Matrixl = cv::Mat(3, 3, CV_32FC1, scalar::all(0));
    cv::Mat rotation_Matrixr = cv::Mat(3, 3, CV_32FC1, scalar::all(0));
    for (ulong i = 0; i < image_numl; i++)
    {
        cout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
        // ofout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
        cout << rvecsMatl[i] << "," << rvecsMatr[i] << endl;
        // ofout << rvecsMatl[i] << "," << rvecsMatr[i] <<endl;
        cout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
        // ofout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
        /**将旋转向量转换为相对应的旋转矩阵*/
        cv::Rodrigues(rvecsMatl[i], rotation_Matrixl);
        cv::Rodrigues(rvecsMatr[i], rotation_Matrixr);
        cout << rotation_Matrixl << "," << rotation_Matrixr << endl;
        // ofout << rotation_Matrixl << "," << rotation_Matrixr <<endl;
        cout << "第" << i + 1 << "幅图像的平移向量：" << endl;
        // ofout << "第" << i + 1 << "幅图像的平移向量：" << endl;
        cout << tvecsMatl[i] << "," << tvecsMatr[i] << endl;
        // ofout << tvecsMatl[i] << "," << tvecsMatr[i] << endl;
    }

    msg[2] = "结果保存完毕";
    //对标定结果进行评价
    msg[3] = "开始评价标定结果......";

    //计算每幅图像中的角点数量，假设全部角点都检测到了
    int corner_points_counts;
    corner_points_counts = ptsz.width * ptsz.height;

    msg[4] = "每幅图像的标定误差：";
    // ofout << "每幅图像的标定误差：" << endl;
    /**单张图像的误差*/
    double err = 0;
    /**所有图像的平均误差*/
    double total_err = 0;
    for (ulong i = 0; i < image_numl; i++)
    { /*存放新计算出的投影点的坐标*/
        vector<cv::Point2f> image_points_calculated;
        vector<cv::Point3f> tempPointSet = objectPoints[i];
        cv::projectPoints(tempPointSet, rvecsMatl[i], tvecsMatl[i], cameraMatrix[0], distCoefficients[0],
                          image_points_calculated);
        /**计算新的投影点与旧的投影点之间的误差*/
        vector<cv::Point2f> image_points_old = cornerpsl[i];
        /**将两组数据换成Mat格式*/
        cv::Mat image_points_calculated_mat = cv::Mat(1, int(image_points_calculated.size()), CV_32FC2);
        cv::Mat image_points_old_mat = cv::Mat(1, int(image_points_old.size()), CV_32FC2);
        for (ulong j = 0; j < tempPointSet.size(); j++)
        {
            image_points_calculated_mat.at<cv::Vec2f>(0, int(j)) =
                cv::Vec2f(image_points_calculated[j].x, image_points_calculated[j].y);
            image_points_old_mat.at<cv::Vec2f>(0, int(j)) = cv::Vec2f(image_points_old[j].x, image_points_old[j].y);
        }
        err = cv::norm(image_points_calculated_mat, image_points_old_mat, cv::NORM_L2);
        err /= corner_points_counts;
        total_err += err;
        cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
        // ofout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
    }
    msg[5] = "总体平均误差：" + str(total_err / image_numl) + "像素";
    // ofout << "总体平均误差：" << total_err / image_numl << "像素" << endl;
    msg[6] = "评价完成";

    // ofout.close();

    cv::Mat mapx = cv::Mat(imgsz, CV_32FC1);
    cv::Mat mapy = cv::Mat(imgsz, CV_32FC1);
    msg[1] = "保存矫正图像";
    string imageFileName;
    stringstream StrStm;
    for (int i = 0; i < int(image_numl); i++)
    {
        cout << "Frame #" << i + 1 << endl;

        cv::initUndistortRectifyMap(cameraMatrix[0], distCoefficients[0], Rl, cameraMatrix[0], imgsz, CV_32FC1, mapx,
                                    mapy);
        cv::Mat src_image = imageInputl[i].clone();
        cv::Mat new_image; // = src_image.clone();
        std::cout << std::endl << imgsz << std::endl;

        cv::remap(src_image, new_image, mapx, mapy, cv::INTER_LINEAR);

        // imshow("原始图像L", src_image);
        // imshow("矫正后图像L", new_image);
        matl = src_image;
        fml = new_image;

        StrStm.clear();
        cv::waitKey(200);
        imageFileName.clear();
        StrStm << i + 1;
        StrStm >> imageFileName;
        imageFileName = "/home/root/eye/dt/em/p" + to_string(0) + "i" + to_string(i) + "_d.jpg ";
        cv::imwrite(imageFileName, new_image);

        cout << "Frame #" << i + 1 << endl;

        cv::initUndistortRectifyMap(cameraMatrix[1], distCoefficients[1], Rr, cameraMatrix[1], imgsz, CV_32FC1, mapx,
                                    mapy);
        src_image = imageInputr[i].clone();
        // = src_image.clone();
        std::cout << std::endl << imgsz << std::endl;

        cv::remap(src_image, new_image, mapx, mapy, cv::INTER_LINEAR);

        // imshow("原始图像R", src_image);
        // imshow("矫正后图像R", new_image);
        matr = src_image;
        fmr = new_image;

        StrStm.clear();
        cv::waitKey(200);

        imageFileName.clear();
        StrStm << i + 1;
        StrStm >> imageFileName;
        imageFileName = "/home/root/eye/dt/em/p" + to_string(1) + "i" + to_string(i) + "_d.jpg ";
        cv::imwrite(imageFileName, new_image);

        cv::waitKey(200);
    }

    cv::waitKey(200);
#if 1
    cv::stereoCalibrate(objectPoints, cornerpsl, cornerpsr,
                        cameraMatrix[0], distCoefficients[0],
                        cameraMatrix[1], distCoefficients[1],
                        matl.size(), R0, T0, E0, F0, pVE/*,CALIB_FIX_PRINCIPAL_POINT,
                        TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,30,1e-6)*/);
#endif
    cv::FileStorage fs("/home/root/eye/dt/em/eyematrix", cv::FileStorage::WRITE);
    fs << "eyematrix0" << cameraMatrix[0] << "eyematrix1" << cameraMatrix[1] << "distCoefficients0"
       << distCoefficients[0] << "distCoefficients1" << distCoefficients[1] << "Rl" << Rl << "Rr" << Rr << "R0"
       << R0 // R
       << "T0" << T0 << "E0" << E0 << "F0" << F0 << "pVE" << pVE;
    fs.release();

    image_numl = 0;
    image_numr = 0;
    corner_points_bufl.clear();
    corner_points_bufr.clear();
    cornerpsl.clear();
    cornerpsr.clear();

    cout << "保存结束" << endl;

    return 0;
}

int d2l::saveEyeMat()
{
    cv::FileStorage fs("/home/root/eye/dt/em/eyematrix2", cv::FileStorage::WRITE);
    fs << "ml" << newml << "mr" << newmr;
    fs.release();
    return 1;
}

int d2l::readEyeMat()
{
    cv::FileStorage fs("/home/root/eye/dt/em/eyematrix2", cv::FileStorage::READ);
    fs["ml"] >> ml;
    fs["mr"] >> mr;
    fs.release();
    return 1;
}

int d2l::findChessboard(cv::Mat &matl, cv::Mat &matr, string msg[], std::vector<vr> &cbc)
{
    using namespace std;
    using namespace cv;
    imgsz.width = matl.cols;
    imgsz.height = matl.rows;
    cbc.clear();
    vr v;

    if (findChessboardCornersSB(matl, ptsz, corner_points_bufl, CALIB_CB_ACCURACY) &&
        findChessboardCornersSB(matr, ptsz, corner_points_bufr, CALIB_CB_ACCURACY))
    {

        ulong bfsz = ulong(ptsz.width * ptsz.height);
        float d = 11.0f;

        for (ulong i = 0; i < bfsz; i++)
        {
            if (corner_points_bufl[i].x < d || corner_points_bufl[i].x > imgsz.width - d ||
                corner_points_bufr[i].x < d || corner_points_bufr[i].x > imgsz.width - d ||
                corner_points_bufl[i].y < d || corner_points_bufl[i].y > imgsz.height - d ||
                corner_points_bufr[i].y < d || corner_points_bufr[i].y > imgsz.height - d)
            {
                return 0; // cvbug:   find4QuadCornerSubpix会报错;
            }
        }
        // cv::find4QuadCornerSubpix(cvtcolor(matl), corner_points_bufl, ptsz);
        // cv::find4QuadCornerSubpix(cvtcolor(matr), corner_points_bufr, ptsz);

        for (ulong i = 0; i < corner_points_bufl.size(); i++)
        {
            v.lp = corner_points_bufl[i];
            v.rp = corner_points_bufr[i];
            cbc.push_back(v);
        }
        corner_points_bufl.clear();
        corner_points_bufr.clear();
        return 1;
    }
    msg[0] = "没看见棋盘格";
    return 0;
}

#include "opencv2/highgui.hpp"
//#include "opencv2/xfeatures2d/nonfree.hpp"
#include <iostream>

#ifndef FYAIRO_1_0_0_ARM
#ifdef OPENCV_CORE_CUDA
void d2::dffd(cv::cuda::GpuMat image01, cv::cuda::GpuMat image02)
{
    using namespace cv;
    using namespace std;

    cv::cuda::GpuMat img1_gray_gpu, img2_gray_gpu;

    cv::cuda::cvtColor(image01, img1_gray_gpu, COLOR_RGB2GRAY);
    cv::cuda::cvtColor(image02, img2_gray_gpu, COLOR_RGB2GRAY);

    cv::Ptr<cv::cuda::ORB> orb = cv::cuda::ORB::create(5000, 1.2f, 8, 31, 0, 2, 0, 31, 20, true);

    cv::cuda::GpuMat keypoints1_gpu, descriptors1_gpu;

    orb->detectAndComputeAsync(img1_gray_gpu, cv::cuda::GpuMat(), keypoints1_gpu, descriptors1_gpu);
    std::vector<cv::KeyPoint> keypoints1;

    orb->convert(keypoints1_gpu, keypoints1);
    /*std::cout << "keypoints1=" << keypoints1.size() << " ; descriptors1_gpu="
              << descriptors1_gpu.rows
              << "x" << descriptors1_gpu.cols << std::endl;*/

    std::vector<cv::KeyPoint> keypoints2;
    cv::cuda::GpuMat descriptors2_gpu;

    orb->detectAndCompute(img2_gray_gpu, cv::cuda::GpuMat(), keypoints2, descriptors2_gpu);
    /*std::cout << "keypoints2=" << keypoints2.size()
              << " ; descriptors2_gpu=" << descriptors2_gpu.rows
              << "x" << descriptors2_gpu.cols << std::endl;*/

    cv::Ptr<cv::cuda::DescriptorMatcher> matcher = cv::cuda::DescriptorMatcher::createBFMatcher(cv::NORM_HAMMING);

    std::vector<std::vector<cv::DMatch>> knn_matches;

    matcher->knnMatch(descriptors2_gpu, descriptors1_gpu, knn_matches, 2);

    std::vector<cv::DMatch> matches;

    vr vmatch;

    cv::Mat imgRes, img1, img2;
    image01.download(img1);
    image02.download(img2);

    int eht = img1.rows;
    for (std::vector<std::vector<cv::DMatch>>::const_iterator it = knn_matches.begin(); it != knn_matches.end(); ++it)
    {
        if (it->size() > 1 && (*it)[0].distance / (*it)[1].distance < 0.6f)
        {
            matches.push_back((*it)[0]);

            vmatch.lp = keypoints1[ulong((*it)[0].queryIdx)].pt;
            vmatch.rp = keypoints2[ulong((*it)[0].trainIdx)].pt;

            if (abs(vmatch.lp.y - vmatch.rp.y) > 4 || vmatch.lp.x < vmatch.rp.x) continue;

            dots[int(eht - vmatch.lp.x)][int(vmatch.lp.y)].dt(vmatch.vo().e());

            /*std::cout << " ["
                     << keypoints1[ulong((*it)[0].trainIdx)].pt
                     << ","
                     << keypoints2[ulong((*it)[1].trainIdx)].pt
                     << "] " ;*/
        }
    }
    std::cout << "[" << matches.size() << "]";

    cv::drawMatches(img2, keypoints2, img1, keypoints1, matches, imgRes);
    cv::imshow("imgRes", imgRes);

    cv::waitKey(3);
}
#endif
#endif

cv::Point d2l::fdr2(cv::Point2d &lp, color9 /**匹配色0*/ &cl, color9 /**匹配色1*/ &cr)
{

    cv::Point2d rp;
    uint fd = 0, lx = uint(lp.x), ly = uint(lp.y), ry = uint(lp.y);

    // double r[2][3]; r[0][0]=3; r[0][1]=3; r[0][2]=3;

    for (uint rx = uint(lp.x - 1); rx > 1; rx--)
    {

        if (cl.c[lx][ly].l && cr.c[rx][ry].l)
        {

            if (cl.c[lx][ly].l == cr.c[rx][ry].l &&

                cl.c[lx][ly].i == cr.c[rx][ry].i &&

                cl.c[lx][ly].like(cr.c[rx][ry].rgb0())

                /* &&cl.c[lx+1][ly  ].like(cr.c[rx+1][ry  ].rgb0(),args[7],args[8])*/
                /* &&cl.c[lx-1][ly  ].like(cr.c[rx-1][ry  ].rgb0(),args[7],args[8])*/

                /*                  &&cl.c[lx  ][ly+2].like(cr.c[rx  ][ry+2].rgb0(),args[7],args[8])
                                  &&cl.c[lx  ][ly-2].like(cr.c[rx  ][ry-2].rgb0(),args[7],args[8])

                                  &&cl.c[lx+2][ly+2].like(cr.c[rx+2][ry+2].rgb0(),args[7],args[8])
                                  &&cl.c[lx-2][ly-2].like(cr.c[rx-2][ry-2].rgb0(),args[7],args[8])

                                  &&cl.c[lx-2][ly+2].like(cr.c[rx+2][ry+2].rgb0(),args[7],args[8])
                                  &&cl.c[lx+2][ly-2].like(cr.c[rx-2][ry-2].rgb0(),args[7],args[8])*/

                /*&&cl.c[lx+1][ly-1].like(cr.c[rx+1][ry-1].rgb0())

                  &&cl.c[lx-1][ly+1].like(cr.c[rx-1][ry+1].rgb0())
                  &&cl.c[lx-1][ly-1].like(cr.c[rx-1][ry-1].rgb0())*/

            )
            {
                rp.x = rx;
                cr.c[rx][ry].i = 1;
                fd++;
                break;
            }
        }
    }
    if (!(fd)) { rp.x = -1; }

    // else std::cout<<" {"<<rp.x<<"}";

    rp.y = int(lp.y);

    return rp;
}

cv::Point d2l::fdr3(cv::Point2d &lp, color9 /**匹配色0*/ &cl, color9 /**匹配色1*/ &cr)
{

    cv::Point2d rp;
    uint fd = 0, lx = uint(lp.x), ly = uint(lp.y), ry = uint(lp.y);

    double r[2][3];
    r[0][0] = 3;
    r[0][1] = 3;
    r[0][2] = 3;

    for (uint rx = uint(lp.x - 1); rx > 1; rx--)
    {

        double r00 = (cl.c[lx][ly].rgb0() - cr.c[rx][ry].rgb0()).dnorm();

        if (r00 < 0.045 && r00 < r[0][0])
        {

            r[0][2] = r[0][1];
            r[0][1] = r[0][0];
            r[0][0] = r00;

            r[1][2] = r[1][1];
            r[1][1] = r[1][0];
            r[1][0] = rx;

            fd++;
        }
    }

    if (!(fd)) { rp.x = -1; }

    // else std::cout<<" {"<<rp.x<<"}";
    rp.y = int(lp.y);

    return rp;
}

bool d2l::imatch(std::vector<cv::Point> a, std::vector<cv::Point> b)
{
    double u = contourArea(a, false) / contourArea(b, false);
    return u > 0.9 && u < 1.1;
}

std::vector<vr> d2l::match(std::vector<cv::Point> a, std::vector<cv::Point> b)
{
    std::vector<vr> mac;
    vr mc1;
    for (ulong i = 0; i < a.size(); i++)
    {
        if (a[i].x == b[i].x && a[i].y == b[i].y)
        {
            mc1.lp = a[i];
            mc1.rp = b[i];
            mac.push_back(mc1);
        }
    }
    return mac;
}

std::vector<std::vector<vr>> d2l::fdcts()
{
    std::vector<std::vector<vr>> mcd;
    for (ulong i = 0; i < contoursl.size(); i++)
    {
        for (ulong j = 0; j < contoursr.size(); j++)
        {
            if (imatch(contoursl[i], contoursr[i])) { mcd.push_back(match(contoursl[i], contoursr[i])); }
        }
    }

    return mcd;
}

void d2l::fdcts(cv::Mat iml, cv::Mat imr)
{
    cv::findContours(iml, contoursl, hierarchyl, cv::RETR_LIST /*cv::RETR_TREE*/, cv::CHAIN_APPROX_SIMPLE,
                     cv::Point(0, 0));
    cv::findContours(imr, contoursr, hierarchyr, cv::RETR_LIST /*cv::RETR_TREE*/, cv::CHAIN_APPROX_SIMPLE,
                     cv::Point(0, 0));
}

std::vector<obj> d2l::fdgochees(cv::Mat iml, cv::Mat imr)
{
    std::vector<obj> chess;
    std::vector<vr> wc, bc;
    std::vector<cv::Point> wcl, wcr, bcl, bcr;
    cv::Mat ccs;
#if 1
    printf("\n");
#endif
    for (ulong i = 0; i < contoursl.size(); i++)
    {
        double a = contourArea(contoursl[i], false);
#if 1
        if (a > 100) printf("<%d>", int(a));
#endif
        if (100 < a && a < 800 && contoursl[i].size() > 7)
        {
            std::vector<cv::Point> rtg;
            cv::RotatedRect rrt = cv::minAreaRect(contoursl[i]);
            cv::Point rtc = rrt.center;
            ccs = sbmat(iml, cv::Rect(rtc - cv::Point(5, 5), rtc + cv::Point(5, 5)));
            scalar cl = cv::mean(ccs);
            if (color(cl).name() == "白") { wcl.push_back(rtc); }
            if (color(cl).name() == "黑") { bcl.push_back(rtc); }
        }
    }
    for (ulong i = 0; i < contoursr.size(); i++)
    {
        double a = contourArea(contoursr[i], false);
        if (80 < a && a < 200 && contoursr[i].size() > 7)
        {
            std::vector<cv::Point> rtg;
            cv::Point rtc = cv::minAreaRect(contoursr[i]).center;
            ccs = sbmat(imr, cv::Rect(rtc - cv::Point(5, 5), rtc + cv::Point(5, 5)));
            scalar cl = cv::mean(ccs);
            if (color(cl).name() == "白") { wcl.push_back(rtc); }
            if (color(cl).name() == "黑") { bcl.push_back(rtc); }
        }
    }

    for (ulong i = 0; i < wcl.size(); i++)
    {
        for (ulong j = 0; j < wcr.size(); j++)
        {
            if (like(wcl[i].y, wcr[j].y, 7) && wcl[i].x > wcr[j].x)
            {
                obj f0;
                f0.v.lp = wcl[i];
                f0.v.rp = wcr[j];
                f0.name = "白棋";
                f0.v2e();
                chess.push_back(f0);
            }
        }
    }

    for (ulong i = 0; i < bcl.size(); i++)
    {
        for (ulong j = 0; j < bcr.size(); j++)
        {
            if (like(bcl[i].y, bcr[j].y, 7) && bcl[i].x > bcr[j].x)
            {
                obj f0;
                f0.v.lp = bcl[i];
                f0.v.rp = bcr[j];
                f0.name = "黑棋";
                f0.v2e();
                chess.push_back(f0);
            }
        }
    }

    return chess;
}

int d2l::makeGrid2(std::vector<std::vector<vr>> ds, fysize gsz, cv::Mat &fml, cv::Mat &fmr, std::vector<fyline> *ll2,
                  std::vector<fyline> *lr2, uint ext, int show)
{

    // 四条边的向量， 大小为小正方形宽度
    p3d a = (ds[1][1].e() - ds[0][0].e()) / double(gsz.h - 1 - 2 * ext),
        b = (ds[0][1].e() - ds[1][0].e()) / double(gsz.h - 1 - 2 * ext),
        c = (ds[1][0].e() - ds[0][0].e()) / double(gsz.w - 1 - 2 * ext),
        d = (ds[0][1].e() - ds[1][1].e()) / double(gsz.w - 1 - 2 * ext);
    // 对于每一列
    for (double i = 0; i < gsz.h; i++)
    {
        axis t; // 每一列的竖线
        t.a = ds[0][0].e() - a * int(ext) + a * i;
        t.b = ds[1][0].e() - b * int(ext) + b * i;
        fyline l, r;
        vr ta(t.a), tb(t.b);
        l.l = cv::Vec4d(ta.lp.x, ta.lp.y, tb.lp.x, tb.lp.y);
        if (show) cv::line(fml, l.pa(), l.pb(), scalar(0, 255, 0));
        ll2[0].push_back(l);

        r.l = cv::Vec4d(ta.rp.x, ta.rp.y, tb.rp.x, tb.rp.y);
        if (show) cv::line(fmr, r.pa(), r.pb(), scalar(0, 255, 0));
        lr2[0].push_back(r);
    }

    // 对于每一行
    for (double i = 0; i < gsz.w; i++)
    {
        axis t; // 横线
        t.a = ds[0][0].e() - c * int(ext) + c * i;
        t.b = ds[1][1].e() - d * int(ext) + d * i;
        fyline l, r;
        vr ta(t.a), tb(t.b);
        l.l = cv::Vec4d(ta.lp.x, ta.lp.y, tb.lp.x, tb.lp.y);
        if (show) cv::line(fml, l.pa(), l.pb(), scalar(0, 255, 255));
        ll2[1].push_back(l);

        r.l = cv::Vec4d(ta.rp.x, ta.rp.y, tb.rp.x, tb.rp.y);
        if (show) cv::line(fmr, r.pa(), r.pb(), scalar(0, 255, 255));
        lr2[1].push_back(r);
    }

    return 1;
}

int d2l::makeGrid3(std::vector<std::vector<vr>> &gdv, std::vector<cv::Point2f> _dsl, std::vector<cv::Point2f> _dsr,
                  cv::Mat &fml, cv::Mat &fmr, uint ext, int show)
{
    std::vector<std::vector<cv::Point2d>> dsl, dsr;
    std::vector<cv::Point2d> dtmp;
    // 串羊肉串
    dtmp.push_back(_dsl[0]);
    dtmp.push_back(_dsl[1]);
    dsl.push_back(dtmp);
    dtmp.clear();
    dtmp.push_back(_dsl[2]);
    dtmp.push_back(_dsl[3]);
    dsl.push_back(dtmp);
    dtmp.clear();

    dtmp.push_back(_dsr[0]);
    dtmp.push_back(_dsr[1]);
    dsr.push_back(dtmp);
    dtmp.clear();
    dtmp.push_back(_dsr[2]);
    dtmp.push_back(_dsr[3]);
    dsr.push_back(dtmp);
    return makeGrid3(gdv, dsl, dsr, fml, fmr, ext, show);
}

int d2l::makeGrid3(std::vector<std::vector<vr>> &gdv, std::vector<std::vector<cv::Point2d>> dsl,
                  std::vector<std::vector<cv::Point2d>> dsr, cv::Mat &fml, cv::Mat &fmr, uint ext, int show)
{

    fysize gsz(gdv.size(), gdv[0].size()); // 格子的宽高

    std::vector<std::vector<vr>> ds; // 角点vr点对 todo: 为啥要用二维
    for (ulong i = 0; i < dsl.size(); i++)
    {
        std::vector<vr> d;
        for (ulong j = 0; j < dsl[0].size(); j++) { d.push_back(vr(dsl[i][j], dsr[i][j])); }
        ds.push_back(d);
    }

    axis e(p3d(0, 0, 0), p3d(1, 1, 0)), f(p3d(1, 0, 1), p3d(0, 1, 1)),
        // 对角线
        t1(ds[0][0].e(), ds[0][1].e()), t2(ds[1][0].e(), ds[1][1].e()),
        // 四条边
        a(ds[0][1].e(), ds[1][0].e()), b(ds[1][1].e(), ds[0][0].e()), c(ds[0][0].e(), ds[1][0].e()),
        d(ds[1][1].e(), ds[0][1].e());
    double d1 = 20, d2 = 20;
    // 检查结果
    if (t1.dst(t2) / mm > d1                                             // 对角线距离
        || abs(a.d() - b.d()) / mm > d2 || abs(c.d() - d.d()) / mm > d2) // 边的长度
        return 0;

    // std::cout<<" @"<<t1.len(t2)/mm<<"mm "<<(a.d()-b.d())/mm<<"mm "<<(c.d()-d.d())/mm<<"mm";

    std::vector<fyline> ll2[2], lr2[2]; // 左右图的线

    makeGrid2(ds, gsz, fml, fmr, ll2, lr2, ext, show);

    return lL4(ll2, lr2, gdv);
}

int d2l::findgrid(cv::Mat &frmlg, cv::Mat &frmrg, std::vector<std::vector<vr>> &gdv)
{
    using namespace cv;
    using namespace std;

    int ewd = frmlg.cols, eht = frmlg.rows, ewd2 = ewd / 2, eht2 = eht / 2;

#if 0
    Mat element = getStructuringElement(MORPH_RECT, Size(1, 1));
    morphologyEx(fml, fml, MORPH_OPEN, element);
    morphologyEx(fmr, fmr, MORPH_OPEN, element);

    element = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(fml, fml, MORPH_CLOSE, element);
    morphologyEx(fmr, fmr, MORPH_CLOSE, element);

    cvtColor(fml,fml0,COLOR_RGB2GRAY);
    cvtColor(fmr,fmr0,COLOR_RGB2GRAY);
#endif

    vector<fyline> ll1, lr1, ll2[2], lr2[2];
    std::vector<cv::Point2d> dl, dr;

    vector<Vec4f> lines_fldl, lines_fldr;

    std::vector<std::vector<cv::Point2d>> dsl1, // 左图角点
        dsr1;                                   // 右图角点

    frmlg2 = cvtcolor(frmlg);
    frmrg2 = cvtcolor(frmrg);

#ifdef fusehoughlinesp
    HoughLinesP(frmlg2, lines_fldl, 1, 1.0 * CV_PI / 180, 80, 25, 25);
    HoughLinesP(frmrg2, lines_fldr, 1, 1.0 * CV_PI / 180, 80, 25, 25);

    dpsm(lines_fldl, ll1, frmlg, dl);
    dpsm(lines_fldr, lr1, frmrg, dr);
#else
    frmlg1 = frmlg.clone();
    frmrg1 = frmrg.clone();
#ifndef FYAIRO_1_0_0_ARM
    // 检测左右图中的线
    fld->detect(
#if 1
        frmlg2
#else
        cvtcolor(fml)
#endif
        ,
        lines_fldl);

    fld->detect(
#if 1
        frmrg2
#else
        cvtcolor(fmr)
#endif
        ,
        lines_fldr);
#endif
#if 0
    for (ulong i = 0; i < lines_fldl.size(); ++i) {
        line( frmlg,cv::Point2f(lines_fldl[i].val[0],lines_fldl[i].val[1]),
                    cv::Point2f(lines_fldl[i].val[2],lines_fldl[i].val[3]),
                rnds());
    }

    for (ulong i = 0; i < lines_fldr.size(); ++i) {
        line( frmrg,cv::Point2f(lines_fldr[i].val[0],lines_fldr[i].val[1]),
                    cv::Point2f(lines_fldr[i].val[2],lines_fldr[i].val[3]),
                rnds());
    }
#endif
    // 找出符合条件的线
    dpsm(lines_fldl, ll1, frmlg, dl);
    dpsm(lines_fldr, lr1, frmrg, dr);

#endif
    // 分成水平和垂直两组
    lgrp(ll1, ll2);
    lgrp(lr1, lr2);

    // 检查非空
    if (!ll2[1].size()) return 0;
    if (!lr2[1].size()) return 0;
#if 0

    for (ulong i = 0; i < ll2[0].size(); ++i) {
        line( frmlg,ll2[0][i].pa(), ll2[0][i].pb(),
                scalar(255,0,0));
    }

    for (ulong i = 0; i < ll2[1].size(); ++i) {
        line( frmlg,ll2[1][i].pa(), ll2[1][i].pb(),
                scalar(0,255,0));
    }

    for (ulong i = 0; i < lr2[0].size(); ++i) {
        line( frmrg,lr2[0][i].pa(), lr2[0][i].pb(),
                scalar(255,0,0));
    }

    for (ulong i = 0; i < lr2[1].size(); ++i) {
        line( frmrg,lr2[1][i].pa(), lr2[1][i].pb(),
                scalar(0,255,0));
    }
#endif

#if 0
    for (ulong i = 0; i < ll2[0].size(); ++i) {
        line( frmlg,ll2[0][i].y(    40), ll2[0][i].y(ewd-40), Scalar(0,0,255));
    }
    for (ulong i = 0; i < ll2[1].size(); ++i) {
        line( frmlg,ll2[1][i].x(    40), ll2[1][i].x(eht-40), Scalar(0,255,0));
    }

    for (ulong i = 0; i < lr2[0].size(); ++i) {
        line( frmrg,lr2[0][i].y(    40), lr2[0][i].y(ewd-40), Scalar(0,0,255));
    }
    for (ulong i = 0; i < lr2[1].size(); ++i) {
        line( frmrg,lr2[1][i].x(    40), lr2[1][i].x(eht-40), Scalar(0,255,0));
    }
#endif

#if 0 /*测试fyline.y(,)*/
    fyline f1;
    f1.l[0]=200+sbsb*10;f1.l[1]=110;f1.l[2]=400-sbsb*10;f1.l[3]=150;
    line(frmlg,f1.pa(),f1.pb(),Scalar(0,255));
    circle(frmlg,f1.y(300),3,Scalar(255));
    circle(frmlg,f1.y(300,50),3,Scalar(255));
#endif

    // 分别对横竖排序
    vector<fyline> ll3[2], lr3[2];
    fyline /**横*/ lh, /**竖*/ lv, l0, l1;

    lh.l[0] = 0;
    lh.l[1] = eht2;
    lh.l[2] = ewd;
    lh.l[3] = eht2;
    lv.l[0] = ewd2;
    lv.l[1] = 0;
    lv.l[2] = ewd2;
    lv.l[3] = eht;

    // 排序
    lst(ll2[0], ll3[0], ll2[1][0]);
    lst(ll2[1], ll3[1], ll2[0][0]);

    lst(lr2[0], lr3[0], lr2[1][0]);
    lst(lr2[1], lr3[1], lr2[0][0]);

#if 0
    for (ulong i = 0; i < ll3[0].size(); ++i) {
        putText(frmlg,str(int(i)),ll3[0][i].pa(),1,1,Scalar(0,0,255));
        line( frmlg,ll3[0][i].pa(), ll3[0][i].pb(),
                /*srnd()*/Scalar(0,0,255));
    }
#endif
#if 0
    for (ulong i = 0; i < ll3[1].size(); ++i) {
        putText(frmlg,str(int(i)),ll3[1][i].pa(),1,1,Scalar(255,0,0));
        line( frmlg,ll3[1][i].pa(), ll3[1][i].pb(),
                /*srnd()*/Scalar(255,0,0));
    }
#endif
#if 0
    for (ulong i = 0; i < lr3[0].size(); ++i) {
        putText(frmrg,str(int(i)),lr3[0][i].pa(),1,1,Scalar(0,0,255));
        line( frmrg,lr3[0][i].pa(), lr3[0][i].pb(),
                /*srnd()*/Scalar(0,0,255));
    }
#endif
#if 0
    for (ulong i = 0; i < lr3[1].size(); ++i) {
        putText(frmrg,str(int(i)),lr3[1][i].pa(),1,1,Scalar(255,0,0));
        line( frmrg,lr3[1][i].pa(), lr3[1][i].pb(),
                /*srnd()*/Scalar(255,0,0));
    }
#endif

    if (ll3[0].size() < 3 || lr3[0].size() < 3) return 0;
    if (ll3[1].size() < 3 || lr3[1].size() < 3) return 0;

    // 边框
    vector<fyline> ll3a, lr3a;

    ll3a.push_back(ll3[0][0]);
    ll3a.push_back(ll3[0][ll3[0].size() - 1]);
    ll3a.push_back(ll3[1][0]);
    ll3a.push_back(ll3[1][ll3[1].size() - 1]);

    lr3a.push_back(lr3[0][0]);
    lr3a.push_back(lr3[0][lr3[0].size() - 1]);
    lr3a.push_back(lr3[1][0]);
    lr3a.push_back(lr3[1][lr3[1].size() - 1]);

#if 0
    for (uint i=0;i<4;i++) {
        line( frmlg,ll3a[i].pa(), ll3a[i].pb(),
                    /*srnd()*/Scalar(255,0,255),1);
        line( frmrg,lr3a[i].pa(), lr3a[i].pb(),
                    /*srnd()*/Scalar(255,0,255),1);
    }
#endif
    vector<Point2d> dtmp;
    // 把边缘四个点推进ds1数组
    // todo: 为啥非得是二维数组
    dtmp.clear();
    dtmp.push_back(ll3a[2].i(ll3a[0]));
    dtmp.push_back(ll3a[3].i(ll3a[1]));
    dsl1.push_back(dtmp);

    dtmp.clear();
    dtmp.push_back(ll3a[0].i(ll3a[3]));
    dtmp.push_back(ll3a[1].i(ll3a[2]));
    dsl1.push_back(dtmp);

    dtmp.clear();
    dtmp.push_back(lr3a[2].i(lr3a[0]));
    dtmp.push_back(lr3a[3].i(lr3a[1]));
    dsr1.push_back(dtmp);

    dtmp.clear();
    dtmp.push_back(lr3a[0].i(lr3a[3]));
    dtmp.push_back(lr3a[1].i(lr3a[2]));
    dsr1.push_back(dtmp);

    // 二维 -> 一维 用于接下来的精确查找
    vector<Point2f> gdvl, gdvr;

    gdvl.push_back(dsl1[0][0]);
    gdvl.push_back(dsl1[0][1]);
    gdvl.push_back(dsl1[1][0]);
    gdvl.push_back(dsl1[1][1]);

    gdvr.push_back(dsr1[0][0]);
    gdvr.push_back(dsr1[0][1]);
    gdvr.push_back(dsr1[1][0]);
    gdvr.push_back(dsr1[1][1]);

    //        画点
    circle(frmlg, dsl1[0][0], 2, scalar(0, 0, 0), 1, LINE_AA);
    circle(frmlg, dsl1[0][1], 2, scalar(255, 0, 0), 1, LINE_AA);
    circle(frmlg, dsl1[1][0], 2, scalar(0, 255, 0), 1, LINE_AA);
    circle(frmlg, dsl1[1][1], 2, scalar(255, 0, 255), 1, LINE_AA);

    circle(frmrg, dsr1[0][0], 2, scalar(0, 0, 0), 1, LINE_AA);
    circle(frmrg, dsr1[0][1], 2, scalar(255, 0, 0), 1, LINE_AA);
    circle(frmrg, dsr1[1][0], 2, scalar(0, 255, 0), 1, LINE_AA);
    circle(frmrg, dsr1[1][1], 2, scalar(255, 0, 255), 1, LINE_AA);

    // 精确查找交叉点
    TermCriteria criteria = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 40, 0.01);

    for (uint i = 0; i < gdvl.size(); i++)
    {
        if (!hav(double(gdvl[i].x), 0, ewd) || !hav(double(gdvl[i].y), 0, eht)) return 0;
    }

    for (uint i = 0; i < gdvr.size(); i++)
    {
        if (!hav(double(gdvr[i].x), 0, ewd) || !hav(double(gdvr[i].y), 0, eht)) return 0;
    }

    cornerSubPix(frmlg2, gdvl, Size(8, 8), Size(-1, -1), criteria);
    cornerSubPix(frmrg2, gdvr, Size(8, 8), Size(-1, -1), criteria);

    // 画小圈
    for (uint i = 0; i < gdvl.size(); i++) { circle(frmlg, gdvl[i], 1, Scalar(0, 0, 255), 2, 8, 0); }

    for (uint i = 0; i < gdvr.size(); i++) { circle(frmrg, gdvr[i], 1, Scalar(0, 0, 255), 2, 8, 0); }

    // 精确查找完赋值回来
    dsl1[0][0] = gdvl[0];
    dsl1[0][1] = gdvl[1];
    dsl1[1][0] = gdvl[2];
    dsl1[1][1] = gdvl[3];

    dsr1[0][0] = gdvr[0];
    dsr1[0][1] = gdvr[1];
    dsr1[1][0] = gdvr[2];
    dsr1[1][1] = gdvr[3];

    int mc0 = makeGrid3(gdv, dsl1, dsr1, frmlg, frmrg, 0, 0);

    if (!mc0) return 0;

    // 赋值小一圈的
    dsl1[0][0] = gdv[1][1].lp;
    dsl1[0][1] = gdv[gdv.size() - 2][gdv[0].size() - 2].lp;
    dsl1[1][0] = gdv[1][gdv[0].size() - 2].lp;
    dsl1[1][1] = gdv[gdv.size() - 2][1].lp;

    dsr1[0][0] = gdv[1][1].rp;
    dsr1[0][1] = gdv[gdv.size() - 2][gdv[0].size() - 2].rp;
    dsr1[1][0] = gdv[1][gdv[0].size() - 2].rp;
    dsr1[1][1] = gdv[gdv.size() - 2][1].rp;

    // 画图
    circle(frmlg, dsl1[0][0], 3, Scalar(0, 0, 0), 2);
    circle(frmlg, dsl1[0][1], 3, Scalar(0, 255, 0), 2);
    circle(frmlg, dsl1[1][0], 3, Scalar(255, 0, 0), 2);
    circle(frmlg, dsl1[1][1], 3, Scalar(255, 255, 0), 2);

    circle(frmrg, dsr1[0][0], 3, Scalar(0, 0, 0), 2);
    circle(frmrg, dsr1[0][1], 3, Scalar(0, 255, 0), 2);
    circle(frmrg, dsr1[1][0], 3, Scalar(255, 0, 0), 2);
    circle(frmrg, dsr1[1][1], 3, Scalar(255, 255, 0), 2);

    gdvl.clear();
    gdvr.clear();

    gdvl.push_back(dsl1[0][0]);
    gdvl.push_back(dsl1[0][1]);
    gdvl.push_back(dsl1[1][0]);
    gdvl.push_back(dsl1[1][1]);

    gdvr.push_back(dsr1[0][0]);
    gdvr.push_back(dsr1[0][1]);
    gdvr.push_back(dsr1[1][0]);
    gdvr.push_back(dsr1[1][1]);

    if (isinf(gdvl[0].x)) return 0;
    if (isinf(gdvr[0].x)) return 0;

    // 找小一圈的角点
    cornerSubPix(frmlg2, gdvl, Size(5, 5), Size(-1, -1), criteria);
    cornerSubPix(frmrg2, gdvr, Size(5, 5), Size(-1, -1), criteria);

    dsl1[0][0] = gdvl[0];
    dsl1[0][1] = gdvl[1];
    dsl1[1][0] = gdvl[2];
    dsl1[1][1] = gdvl[3];

    dsr1[0][0] = gdvr[0];
    dsr1[0][1] = gdvr[1];
    dsr1[1][0] = gdvr[2];
    dsr1[1][1] = gdvr[3];

    circle(frmlg, dsl1[0][0], 1, Scalar(255, 255, 255), 2);
    circle(frmlg, dsl1[0][1], 1, Scalar(0, 100, 100), 2);
    circle(frmlg, dsl1[1][0], 1, Scalar(100, 0, 100), 2);
    circle(frmlg, dsl1[1][1], 1, Scalar(100, 100, 100), 2);

    circle(frmrg, dsr1[0][0], 1, Scalar(255, 255, 255), 2);
    circle(frmrg, dsr1[0][1], 1, Scalar(0, 100, 100), 2);
    circle(frmrg, dsr1[1][0], 1, Scalar(100, 0, 100), 2);
    circle(frmrg, dsr1[1][1], 1, Scalar(100, 100, 100), 2);

    mc0 = makeGrid3(gdv, dsl1, dsr1, frmlg, frmrg, 1, 1);

    return mc0;
}

int d2l::FindGridHough(cv::Mat &frml, cv::Mat &frmr, std::vector<std::vector<vr>> &gdv)
{
    using namespace cv;
    using namespace std;
    static Mat frmlg, frmrg;              // 左右灰度图
    int ewd = frml.cols, eht = frml.rows; // 图像的行列
    int ewd2 = ewd / 2, eht2 = eht / 2;
    //    imwrite("ChessBoard.jpg", frml); // 获取一张测试用图

    // 灰度化
    cvtColor(frml, frmlg, COLOR_BGR2GRAY);
    cvtColor(frmr, frmrg, COLOR_BGR2GRAY);

    // 寻找网格线段
    vector<fyline> fylines[2]; // 0 左 1 右
    bool fylines_check;
    fylines_check = HoughTransform(frmlg, fylines[0]);
    fylines_check = HoughTransform(frmrg, fylines[1]) & fylines_check;
    if (!fylines_check) return 0;
    // 水平竖直分组 顺序不定
    vector<fyline> fylines_split[2][2];
    lgrp(fylines[0], fylines_split[0]);
    lgrp(fylines[1], fylines_split[1]);

    // 排序
    lst(fylines_split[0][0], fylines_split[0][0], fylines_split[0][1][0]);
    lst(fylines_split[0][1], fylines_split[0][1], fylines_split[0][0][0]);
    lst(fylines_split[1][0], fylines_split[1][0], fylines_split[1][1][0]);
    lst(fylines_split[1][1], fylines_split[1][1], fylines_split[1][0][0]);

    // 检查线段数量
    if (fylines_split[0][0].size() != 9 || fylines_split[0][1].size() != 9 || fylines_split[1][0].size() != 9 ||
        fylines_split[1][1].size() != 9)
        return 0;
    // 获取边框
    vector<fyline> edges[2]; // 0 左 1 右
    for (int i = 0; i < 2; i++)
    {
        edges[i].push_back(fylines_split[i][0].front());
        edges[i].push_back(fylines_split[i][0].back());
        edges[i].push_back(fylines_split[i][1].front());
        edges[i].push_back(fylines_split[i][1].back());
    }

    // 获取角点粗略位置
    std::vector<Point2f> corner_points[2];
    for (int i = 0; i < 2; i++)
    {
        corner_points[i].push_back(edges[i][0].i(edges[i][2]));
        corner_points[i].push_back(edges[i][1].i(edges[i][3]));
        corner_points[i].push_back(edges[i][0].i(edges[i][3]));
        corner_points[i].push_back(edges[i][1].i(edges[i][2]));
    }

    // 粗略角点绘制
    for (const auto &corner_point : corner_points[0]) { circle(frml, corner_point, 10, fc_red, 1, LINE_AA); }
    for (const auto &corner_point : corner_points[1]) { circle(frmr, corner_point, 10, fc_red, 1, LINE_AA); }

    // 精确查找交叉点
    TermCriteria criteria = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 40, 0.01);
    cornerSubPix(frmlg, corner_points[0], Size(8, 8), Size(-1, -1), criteria);
    cornerSubPix(frmrg, corner_points[1], Size(8, 8), Size(-1, -1), criteria);

    // 精确角点绘制
    for (const auto &corner_point : corner_points[0]) { circle(frml, corner_point, 4, fc_green, 1, LINE_AA); }
    for (const auto &corner_point : corner_points[1]) { circle(frmr, corner_point, 4, fc_green, 1, LINE_AA); }

    // 获取三维棋盘格
    int mc0 = makeGrid3(gdv, corner_points[0], corner_points[1], frml, frmr, 0, 0);
    if (mc0 == 0) return mc0;
    // 小一圈的角点
    std::vector<Point2f> inner_corner_points[2];
    inner_corner_points[0].push_back(gdv[1][1].lp);
    inner_corner_points[0].push_back(gdv[gdv.size() - 2][gdv[0].size() - 2].lp);
    inner_corner_points[0].push_back(gdv[1][gdv[0].size() - 2].lp);
    inner_corner_points[0].push_back(gdv[gdv.size() - 2][1].lp);
    inner_corner_points[1].push_back(gdv[1][1].rp);
    inner_corner_points[1].push_back(gdv[gdv.size() - 2][gdv[0].size() - 2].rp);
    inner_corner_points[1].push_back(gdv[1][gdv[0].size() - 2].rp);
    inner_corner_points[1].push_back(gdv[gdv.size() - 2][1].rp);

    // 粗略角点绘制
    for (const auto &inner_corner_point : inner_corner_points[0])
    {
        circle(frml, inner_corner_point, 10, fc_red, 1, LINE_AA);
    }
    for (const auto &inner_corner_point : inner_corner_points[1])
    {
        circle(frmr, inner_corner_point, 10, fc_red, 1, LINE_AA);
    }

    // 精确交点查找
    cornerSubPix(frmlg, inner_corner_points[0], Size(8, 8), Size(-1, -1), criteria);
    cornerSubPix(frmrg, inner_corner_points[1], Size(8, 8), Size(-1, -1), criteria);

    // 精确角点绘制
    for (const auto &inner_corner_point : inner_corner_points[0])
    {
        circle(frml, inner_corner_point, 4, fc_green, 1, LINE_AA);
    }
    for (const auto &inner_corner_point : inner_corner_points[1])
    {
        circle(frmr, inner_corner_point, 4, fc_green, 1, LINE_AA);
    }
    mc0 = makeGrid3(gdv, inner_corner_points[0], inner_corner_points[1], frml, frmr, 1, 1);
#if 0 || DBG_CHESSBD_DETECT
#if 0 || DBG_CHESSBD_DETECT_PRE
    // Draw the lines
    Mat cannyl = frmlb.clone();
    cvtColor(cannyl, cannyl, COLOR_GRAY2BGR);
    for (auto l : lines[0]) { line(cannyl, Point(l[0], l[1]), Point(l[2], l[3]), colors[2], 1, LINE_AA); }
    imshow("left", cannyl);
    imshow("right", frmrb);
#endif
#if 1 || DBG_CHESSBD_DETECT_GRP
    for (auto &l : fylines_split[0][0]) { line(frml, l.pa(), l.pb(), colors[3]); }
    for (auto &l : fylines_split[0][1]) { line(frml, l.pa(), l.pb(), colors[0]); }
    for (auto &l : fylines_split[1][0]) { line(frmr, l.pa(), l.pb(), colors[3]); }
    for (auto &l : fylines_split[1][1]) { line(frmr, l.pa(), l.pb(), colors[0]); }
    imshow("left", frml);
    imshow("right", frmr);
#endif
    waitKey(20);
#endif
    return mc0;
}

/**
 * @brief 形态学操作
 * @param _src
 * @param _dst
 * @param operation see #MorphTypes
 * @param morph_type Element: 0: Rect 1: Cross 2: Ellipse, see #MorphShapes
 * @param morh_size size of window, default 2
 */
void Morphology_Operations(cv::InputArray _src, cv::OutputArray _dst, int operation, int morph_elem,
                           uint morph_size = 2)
{
    cv::Mat src = _src.getMat(), dst = _dst.getMat();
    cv::Mat element = getStructuringElement(morph_elem, cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                                            cv::Point(morph_size, morph_size));
    morphologyEx(src, dst, operation, element);
}

/**
 * @brief 膨胀或收缩
 * @param _src
 * @param _dst
 * @param shape #MorphShapes
 * @param size size of window, default 1
 * @param is_erode bool default false, true for erosion
 */
void ChangeLineWidth(const cv::Mat &src, cv::Mat &dst, int shape, int size = 1, bool is_erode = false)
{
    cv::Mat element = getStructuringElement(shape, cv::Size(2 * size + 1, 2 * size + 1), cv::Point(size, size));
    if (is_erode)
        cv::erode(src, dst, element);
    else
        cv::dilate(src, dst, element);
}

void d2l::HoughPreProcess(const cv::Mat &src, cv::Mat &dst)
{
    using namespace cv;
    CV_Assert(src.channels() == 1); // 灰度图
    dst = src.clone();
    // 直方图均衡 加对比度
    equalizeHist(dst, dst);
    // 高斯模糊
    static const Size size(5, 5);
    GaussianBlur(dst, dst, size, 0);
    // 拉普拉斯
    Laplacian(dst, dst, 8, 3);
    // 二值化
//    adaptiveThreshold(dst, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 5, 2);
    threshold(dst, dst, 0, 255, THRESH_OTSU); // 大津算法
    // 开闭运算
    Morphology_Operations(dst, dst, MORPH_CLOSE, MORPH_RECT, 2);
}

bool d2l::HoughTransform(cv::InputArray _src, std::vector<fyline> &fylines)
{
    using namespace cv;
    const Mat src = _src.getMat();
    CV_Assert(src.channels() == 1); // 灰度图
    Mat img_p;
    HoughPreProcess(src, img_p);
    int check_state = -1;
    for (int operation_cnt = 0; operation_cnt < 3; operation_cnt++)
    {
        Mat img_tmp;
        if (check_state == 1)
            ChangeLineWidth(img_p, img_tmp, MORPH_CROSS, 1);
        else if (check_state == 2)
            ChangeLineWidth(img_p, img_tmp, MORPH_CROSS, 1, true);
        else
            img_tmp = img_p.clone();
        std::vector<Vec4f> lines;
        // 霍夫变换
        HoughLinesP(img_tmp, lines, 1, CV_PI / 180, 100, 25, 10);
        // 检查线段
        check_state = BoardLinesFilter(lines, fylines);
        if (check_state == 0) return true;
    }
    return false;
}
