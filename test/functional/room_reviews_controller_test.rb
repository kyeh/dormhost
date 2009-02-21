require 'test_helper'

class RoomReviewsControllerTest < ActionController::TestCase
  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:room_reviews)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create room_review" do
    assert_difference('RoomReview.count') do
      post :create, :room_review => { }
    end

    assert_redirected_to room_review_path(assigns(:room_review))
  end

  test "should show room_review" do
    get :show, :id => room_reviews(:one).id
    assert_response :success
  end

  test "should get edit" do
    get :edit, :id => room_reviews(:one).id
    assert_response :success
  end

  test "should update room_review" do
    put :update, :id => room_reviews(:one).id, :room_review => { }
    assert_redirected_to room_review_path(assigns(:room_review))
  end

  test "should destroy room_review" do
    assert_difference('RoomReview.count', -1) do
      delete :destroy, :id => room_reviews(:one).id
    end

    assert_redirected_to room_reviews_path
  end
end
