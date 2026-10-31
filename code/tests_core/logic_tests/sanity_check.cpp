#include <gtest/gtest.h>
#include "RubiksCubeLogic.hpp"

// בדיקת מצב התחלתי
TEST(RubiksCubeTest, InitialState) {
    RubiksCubeLogic cube;
    EXPECT_EQ(cube.getHistorySize(), 0);
    EXPECT_EQ(cube.getCubies().size(), 27);
}

// בדיקת סיבוב וביטולו (Undo)
TEST(RubiksCubeTest, RotationAndUndoReturnsToInitialState) {
    RubiksCubeLogic cube;
    float pi = 3.1415926535f;

    // שמירת מצב הקוביות ההתחלתי לצורך השוואה בהמשך
    auto initialCubies = cube.getCubies();

    // 1. ביצוע סיבוב פאה עליונה ב-90 מעלות
    MoveStep testStep = {MoveOption::UP, pi / 2.0f};
    cube.applyLogicStep(testStep);
    EXPECT_EQ(cube.getHistorySize(), 1);

    // 2. ביטול המהלך (Undo)
    cube.undoLogicStep();
    EXPECT_EQ(cube.getHistorySize(), 0);

    // 3. אימות שכל תתי-הקוביות חזרו בדיוק למיקומן ההתחלתי
    const auto& restoredCubies = cube.getCubies();
    ASSERT_EQ(restoredCubies.size(), initialCubies.size());

    for (size_t i = 0; i < restoredCubies.size(); ++i) {
        EXPECT_NEAR(restoredCubies[i].position.x, initialCubies[i].position.x, 1e-4);
        EXPECT_NEAR(restoredCubies[i].position.y, initialCubies[i].position.y, 1e-4);
        EXPECT_NEAR(restoredCubies[i].position.z, initialCubies[i].position.z, 1e-4);
    }
}
