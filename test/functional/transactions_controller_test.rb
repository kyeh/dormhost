require 'test_helper'

class TransactionsControllerTest < ActionController::TestCase
  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:transactions)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create transaction" do
    assert_difference('Transaction.count') do
      post :create, :transaction => { }
    end

    assert_redirected_to transaction_path(assigns(:transaction))
  end

  test "should show transaction" do
    get :show, :id => transactions(:one).id
    assert_response :success
  end

  test "should get edit" do
    get :edit, :id => transactions(:one).id
    assert_response :success
  end

  test "should update transaction" do
    put :update, :id => transactions(:one).id, :transaction => { }
    assert_redirected_to transaction_path(assigns(:transaction))
  end

  test "should destroy transaction" do
    assert_difference('Transaction.count', -1) do
      delete :destroy, :id => transactions(:one).id
    end

    assert_redirected_to transactions_path
  end
end
